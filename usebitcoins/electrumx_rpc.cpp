#include <algorithm>
#include <array>
#include <electrumx_rpc.hpp>
#include <iostream>
#include <memory>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <openssl/x509v3.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <stdexcept>
#include <string_view>
#include <utility>
#include <vector>

namespace usebitcoins
{
namespace electrumx_rpc
{

namespace
{

static ::rapidjson::MemoryPoolAllocator<::rapidjson::CrtAllocator>
	rapidjson_allocator{};

auto ssl_ptr(::BIO* const bio) -> ::SSL*
{
    SSL* ssl = nullptr;
    BIO_get_ssl(bio, &ssl);
    if (ssl == nullptr) {
	ERR_print_errors_fp(::stderr);
	throw std::runtime_error{"BIO_get_ssl() error"};
    }
    return ssl;
}

auto verify_certificate(::SSL* ssl, std::string_view host) -> void
{
    auto err = ::SSL_get_verify_result(ssl);
    if (err != X509_V_OK) {
	auto* s = X509_verify_cert_error_string(err);
	std::cerr << s << std::endl;
	// throw std::runtime_error{s};
    }
    auto* cert = ::SSL_get_peer_certificate(ssl);
    if (cert == nullptr) {
	throw std::runtime_error{"No certificate found"};
    }
    if (1 != ::X509_check_host(cert, host.data(), host.size(), 0, nullptr)) {
	// throw std::runtime_error{"Certificate verification error"};
    }
}

static BIO* bio_err = ::BIO_new_fp(::stderr, BIO_NOCLOSE);

auto call(const electrumx_server_params_t& server,
	  const ::rapidjson::StringBuffer& json_string) -> ::rapidjson::Document
{
    std::unique_ptr<SSL_CTX, decltype(&::SSL_CTX_free)> ssl_ctx{
	    ::SSL_CTX_new(TLS_client_method()), &::SSL_CTX_free};
    // std::unique_ptr<SSL_CTX, decltype(&::SSL_CTX_free)> ssl_ctx{
    // 	    ::SSL_CTX_new(SSLv23_client_method()), &::SSL_CTX_free};
    ::SSL_CTX_set_default_verify_paths(ssl_ctx.get());
    ::SSL_CTX_set_info_callback(
	    ssl_ctx.get(), +[](const SSL* s, int where, int ret) -> void {
		const char* str;
		int w = where & ~SSL_ST_MASK;

		if (w & SSL_ST_CONNECT)
		    str = "SSL_connect";
		else if (w & SSL_ST_ACCEPT)
		    str = "SSL_accept";
		else
		    str = "undefined";

		if (where & SSL_CB_LOOP) {
		    BIO_printf(bio_err, "%s:%s\n", str,
			       SSL_state_string_long(s));
		} else if (where & SSL_CB_ALERT) {
		    str = (where & SSL_CB_READ) ? "read" : "write";
		    BIO_printf(bio_err, "SSL3 alert %s:%s:%s\n", str,
			       SSL_alert_type_string_long(ret),
			       SSL_alert_desc_string_long(ret));
		} else if (where & SSL_CB_EXIT) {
		    if (ret == 0) {
			BIO_printf(bio_err, "%s:failed in %s\n", str,
				   SSL_state_string_long(s));
		    } else if (ret < 0) {
			BIO_printf(bio_err, "%s:error in %s\n", str,
				   SSL_state_string_long(s));
		    }
		}
	    });
    // use OpenSSL's BIO interface as an abstraction over TCP/IP
    std::unique_ptr<::BIO, decltype(&::BIO_free)> bio{
	    ::BIO_new(::BIO_s_connect()), &::BIO_free};
    if (bio.get() == nullptr) {
	throw std::runtime_error{"Failed to initialize BIO"};
    }
    auto hostname = BIO_set_conn_hostname(bio.get(), server.hostname.c_str());
    if (!hostname) {
	throw std::runtime_error{"Failed to set hostname"};
    }
    auto port = BIO_set_conn_port(bio.get(), server.port.c_str());
    if (!port) {
	throw std::runtime_error{"Failed to set port"};
    }
    auto res = BIO_do_connect(bio.get());
    if (res <= 0) {
	ERR_print_errors_fp(::stderr);
	throw std::runtime_error{"BIO_do_connect failed"};
    }
    // TLS handshake
    std::unique_ptr<::BIO, decltype(&::BIO_free)> bio_ssl{
	    ::BIO_new_ssl(ssl_ctx.get(), 1), &::BIO_free};
    bio_ssl.reset(BIO_push(bio_ssl.release(), bio.get()));
    ::SSL_set_tlsext_host_name(ssl_ptr(bio_ssl.get()), server.hostname.c_str());
    ::SSL_set1_host(ssl_ptr(bio_ssl.get()), server.hostname.c_str());
    if (auto res = BIO_do_handshake(bio_ssl.get()); res <= 0) {
	throw std::runtime_error{"Failed TLS handshake"};
    }
    verify_certificate(ssl_ptr(bio_ssl.get()), server.hostname);
    // read response
    std::cout << json_string.GetString() << std::endl;
    ::BIO_write(bio_ssl.get(), json_string.GetString(), json_string.GetSize());
    ::BIO_puts(bio_ssl.get(), "\n");
    BIO_flush(bio_ssl.get());
    std::vector<char> raw_response{};
    for (std::array<char, 1024> buf{};;) {
	size_t read = 0;
	::BIO_read_ex(bio_ssl.get(), buf.data(), buf.size(), &read);
	raw_response.insert(raw_response.end(), buf.begin(),
			    buf.begin() + read);
	if (read <= buf.size()) {
	    break;
	}
    }
    raw_response.push_back('\0');
    ::rapidjson::Document d{};
    d.SetObject();
    d.Parse(raw_response.data());
    return d;
}

auto build_request(::rapidjson::StringBuffer& out, const char* method,
		   const std::vector<::rapidjson::Value>& params) -> void
{
    ::rapidjson::Document d{};
    d.SetObject();
    d.AddMember("id",
		rapidjson::Value{::rapidjson::kStringType}.SetString(
			"usebitcoins_call"),
		d.GetAllocator());
    d.AddMember("method",
		rapidjson::Value{::rapidjson::kStringType}.SetString(
			::rapidjson::StringRef(method)),
		d.GetAllocator());
    ::rapidjson::Value req_params{::rapidjson::kArrayType};
    for (const auto& param : params) {
	req_params.PushBack(const_cast<::rapidjson::Value&>(param),
			    d.GetAllocator());
    }
    d.AddMember("params", req_params, d.GetAllocator());
    ::rapidjson::Writer<::rapidjson::StringBuffer> writer{out};
    d.Accept(writer);
}

} // namespace

auto estimate_fee(const electrumx_server_params_t& server, int conf_target)
	-> uint64_t
{
    static constexpr auto SATOSHIS_PER_BTC = 1e+8f;
    static constexpr auto BYTES_PER_KILOBYTE = 1.24e+3f;
    ::rapidjson::StringBuffer req_str{};
    std::vector<::rapidjson::Value> req_params{};
    ::rapidjson::Value conf_target_param{::rapidjson::kNumberType};
    conf_target_param.SetInt(conf_target);
    req_params.emplace_back(std::move(conf_target_param));
    build_request(req_str, "blockchain.estimatefee", req_params);
    auto res = call(server, req_str);
    if (!res.HasMember("result")) {
	// TODO: handle error
	return 0UL;
    }
    auto btc_per_kilobyte = res["result"].GetDouble();
    auto sats_per_byte =
	    btc_per_kilobyte * SATOSHIS_PER_BTC / BYTES_PER_KILOBYTE;
    return std::max(1UL, static_cast<uint64_t>(sats_per_byte));
}

} // namespace electrumx_rpc
} // namespace usebitcoins

#include "usebitcoins/electrumx_rpc.hpp"
#include <algorithm>
#include <array>
#include <iostream>
#include <memory>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <openssl/x509_vfy.h>
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

// BIO* BIO_new_ssl(SSL_CTX* ctx, int client)
// {
//     BIO* ret;
//     SSL* ssl;

//     if ((ret = BIO_new(BIO_f_ssl())) == NULL)
// 	return NULL;
//     if ((ssl = SSL_new(ctx)) == NULL) {
// 	BIO_free(ret);
// 	return NULL;
//     }
//     if (client)
// 	SSL_set_connect_state(ssl);
//     else
// 	SSL_set_accept_state(ssl);

//     BIO_set_ssl(ret, ssl, BIO_CLOSE);
//     return ret;
// }

static ::rapidjson::MemoryPoolAllocator<::rapidjson::CrtAllocator>
	rapidjson_allocator{};

auto call(const electrumx_server_params_t& server,
	  const ::rapidjson::StringBuffer& json_string) -> ::rapidjson::Document
{
    std::unique_ptr<SSL_CTX, decltype(&::SSL_CTX_free)> ssl_ctx{
	    ::SSL_CTX_new(TLS_client_method()), &::SSL_CTX_free};
    ::SSL_CTX_set_default_verify_paths(ssl_ctx.get());
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
    std::unique_ptr<::SSL, decltype(&::SSL_free)> ssl {::SSL_new(ssl_ctx.get()), &::SSL_free};
    ::SSL_set_connect_state(ssl.get()); // configures as client
    // transfer ownership of `bio` to `ssl`
    auto* bio_ptr = bio.release();
    ::SSL_set_bio(ssl.get(), bio_ptr, bio_ptr);
    ::SSL_set_tlsext_host_name(ssl.get(), server.hostname.c_str());
    // do handshake
    if (int client_err = ::SSL_get_error(ssl.get(), ::SSL_do_handshake(ssl.get()));
	client_err != SSL_ERROR_NONE && client_err != SSL_ERROR_WANT_READ &&
	client_err != SSL_ERROR_WANT_WRITE &&
	client_err != SSL_ERROR_PENDING_TICKET) {
	fprintf(stderr, "Client error (%d): %s\n", client_err,
		::SSL_error_description(client_err));
	throw std::runtime_error{"Failed ssl handshake"};
    }
    ::SSL_write(ssl.get(), json_string.GetString(), json_string.GetSize());
    // ElectrumX JSON-RPC requires a newline
    ::SSL_write(ssl.get(), "\n", 1);
    std::vector<char> raw_response{};
    for (std::array<char, 1024> buf{};;) {
	size_t read = ::SSL_read(ssl.get(), buf.data(), buf.size());
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

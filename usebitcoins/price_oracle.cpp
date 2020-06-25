#include "usebitcoins/price_oracle.hpp"
#include "usebitcoins/mozilla_cacert.hpp"
#include <algorithm>
#include <curl/curl.h>
#include <iostream>
#include <memory>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <rapidjson/document.h>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

namespace usebitcoins
{
namespace price_oracle
{

namespace
{
/// Helper function to use an in-memory CA certificate bundle
/// https://curl.haxx.se/libcurl/c/cacertinmem.html
::CURLcode sslctx_function(::CURL* curl, void* sslctx, void* parm)
{
    ::CURLcode rv = CURLE_ABORTED_BY_CALLBACK;

    ::BIO* cbio = BIO_new_mem_buf(MOZILLA_CACERT, sizeof(MOZILLA_CACERT));
    ::X509_STORE* cts = SSL_CTX_get_cert_store((::SSL_CTX*)sslctx);
    STACK_OF(X509_INFO) * inf;
    (void)curl;
    (void)parm;

    if (!cts || !cbio) {
	return rv;
    }

    inf = PEM_X509_INFO_read_bio(cbio, NULL, NULL, NULL);

    if (!inf) {
	BIO_free(cbio);
	return rv;
    }

    for (size_t i = 0; i < ::sk_X509_INFO_num(inf); i++) {
	::X509_INFO* itmp = ::sk_X509_INFO_value(inf, i);
	if (itmp->x509) {
	    ::X509_STORE_add_cert(cts, itmp->x509);
	}
	if (itmp->crl) {
	    ::X509_STORE_add_crl(cts, itmp->crl);
	}
    }

    ::sk_X509_INFO_pop_free(inf, ::X509_INFO_free);
    ::BIO_free(cbio);

    rv = CURLE_OK;
    return rv;
}
} // namespace

// Calculate the Bitcoin price as a time-weighted average price (TWAP) or
// volume-weighted average price (VWAP) over the past 24 hours. Samples of the
// price every hour are averaged to determine the TWAP; other options are
// possible such as over the past hour by sampling every minute.
//

// TODO(zds): Provide option for pricing-in-bitcoin instead of pricing in fiat

/// Timestamps are UNIX epochs in seconds since 1 Jan 1970
using timestamp_t = uint64_t;

struct timestamped_price_t {
    fiat_price_t price;
    timestamp_t time_recorded;
};

/// Calculate the time weighted average price from a listing of recent prices
fiat_price_t twap(std::vector<timestamped_price_t> price_feed,
		  timestamp_t begin_time, timestamp_t end_time,
		  uint32_t interval_in_minutes = 60)
{
    fiat_price_t twap = 0;
    // sort by time descending
    std::sort(price_feed.begin(), price_feed.end(),
	      [](const timestamped_price_t& a, const timestamped_price_t& b)
		      -> bool { return a.time_recorded > b.time_recorded; });
    timestamp_t prev = 0;
    uint32_t n_samples = 0;
    for (const auto& p : price_feed) {
	if ((p.time_recorded - prev) / 60 >= interval_in_minutes) {
	    ++n_samples;
	    twap = (twap + p.price) / (twap > 0 ? 2 : 1);
	    //                        ^^^^^^^^^^^^^^^^^^
	    //                        so as to not include 0 as one of the
	    //                        prices
	}
	prev = p.time_recorded;
    }
    uint32_t n_expected_samples =
	    (end_time - begin_time) / (interval_in_minutes * 60);
    if (n_samples < n_expected_samples) {
	throw std::runtime_error{
		"Not enough price samples were given to calculate TWAP over "
		"the specified time period and intervals"};
    }
    return twap;
}

/// Fetch price feed from Gemini
std::vector<timestamped_price_t> prices_from_gemini()
{
    // TODO(zds): integrate asio for non-blocking http request
    // fetch("https://api.gemini.com/v2/ticker/btcusd");
    return {};
}

// Fetch price feed from BitMEX
std::vector<timestamped_price_t> prices_from_bitmex()
{
    // TODO(zds): integrate asio for non-blocking http requests
    // TODO(zds): integrate BitMEX's REST API
    return {};
}

std::string GET_as_string(const char* url)
{
    std::unique_ptr<CURL, decltype(&::curl_easy_cleanup)> curl_handle{
	    ::curl_easy_init(), &::curl_easy_cleanup};
    std::string sink{};
    ::curl_easy_setopt(curl_handle.get(), CURLOPT_SSL_VERIFYPEER, 1);
    // use in-memory CA cert bundle
    ::curl_easy_setopt(curl_handle.get(), CURLOPT_SSL_CTX_FUNCTION,
		       *sslctx_function);
    ::curl_easy_setopt(curl_handle.get(), CURLOPT_SSLCERTTYPE, "PEM");
    // do not use system CA cert
    ::curl_easy_setopt(curl_handle.get(), CURLOPT_CAINFO, nullptr);
    ::curl_easy_setopt(curl_handle.get(), CURLOPT_CAPATH, nullptr);
    ::curl_easy_setopt(curl_handle.get(), CURLOPT_URL, url);
    ::curl_easy_setopt(curl_handle.get(), CURLOPT_WRITEDATA, &sink);
    curl_write_callback cb = [](char* buffer, size_t size, size_t nmemb,
				void* userp) -> size_t {
	auto realsize = size * nmemb;
	auto& sink = *static_cast<std::string*>(userp);
	sink.append(buffer, realsize);
	return realsize;
    };
    ::curl_easy_setopt(curl_handle.get(), CURLOPT_WRITEFUNCTION, cb);
    auto res = ::curl_easy_perform(curl_handle.get());
    if (res != CURLE_OK) {
	// TODO: handle error
	throw std::runtime_error{""};
    }
    return sink;
}

fiat_price_t coinbase_price_oracle_t::price() const
{
    if (coinbase_price_oracle_t::throttled()) {
	// TODO: handle control flow for rate limiting
	return 0;
    }
    const auto s = GET_as_string(
	    "https://api.coinbase.com/v2/exchange-rates?currency=BTC");
    // update last request time (for rate limiting purposes)
    coinbase_price_oracle_t::update_last_request_time();
    if (s.size() == 0)
	throw new std::runtime_error{""};
    rapidjson::Document d{};
    d.Parse(s.c_str());
    if (d.HasParseError())
	throw new std::runtime_error{""};
    // returns exchange rates as strings of decimal-formatted currency amounts
    // in JSON object
    const auto& exchange_rates_table = d["data"]["rates"];
    if (!exchange_rates_table.IsObject()) {
	// TODO: handle error
	throw new std::runtime_error{""};
    }
    switch (base_currency_) {
	case fiat_currency_t::USD: {
	    const auto& price_node = exchange_rates_table["USD"];
	    if (!price_node.IsString()) {
		// TODO: handle error
		throw new std::runtime_error{""};
	    }
	    // convert to cents
	    std::cout << price_node.GetString() << std::endl;
	    fiat_price_t fiat_price = static_cast<fiat_price_t>(
		    std::stod(price_node.GetString()) * 100);
	    return fiat_price;
	}
	default:
	    throw new std::runtime_error{
		    "Coinbase price API does not provide an exchange rate for "
		    "this fiat currency pair"};
    }
}

bool coinbase_price_oracle_t::throttled()
{
    const auto now = std::chrono::system_clock::now();
    const auto seconds_elapsed =
	    std::chrono::duration_cast<std::chrono::seconds>(
		    now - coinbase_price_oracle_t::last_request_time_)
		    .count();
    static constexpr auto min_seconds_between_requests = 1;
    return seconds_elapsed < min_seconds_between_requests;
}

void coinbase_price_oracle_t::update_last_request_time()
{
    coinbase_price_oracle_t::last_request_time_ =
	    std::chrono::system_clock::now();
}

} // namespace price_oracle
} // namespace usebitcoins

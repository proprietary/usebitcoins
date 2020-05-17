#include <price_oracle.hpp>
#include <string>
#include <vector>
#include <algorithm>
#include <tuple>

// Calculate the Bitcoin price as a time-weighted average price (TWAP) over the past 24 hours. Samples of the price every hour are averaged to determine the TWAP; other options are possible such as over the past hour by sampling every minute.
//

// TODO(zds): Provide option for pricing-in-bitcoin instead of pricing in fiat

/// Timestamps are UNIX epochs in seconds since 1 Jan 1970
using timestamp_t = uint64_t;

struct timestamped_price_t {
	fiat_price_t price;
	timestamp_t time_recorded;
};


/// Calculate the time weighted average price from a listing of recent prices
fiat_price_t twap(std::vector<timestamped_price_t> price_feed, timestamp_t begin_time, timestamp_t end_time, uint32_t interval_in_minutes = 60) {
	fiat_price_t twap = 0;
	// sort by time descending
	std::sort(price_feed.begin(), price_feed.end(), [](const timestamped_price_t& a, const timestamped_price_t& b) -> bool {
			return a.timestamp_t > b.timestamp_t;
			});
	timestamp_t prev = 0;
	uint32_t n_samples = 0;
	for (const auto& p : price_feed) {
		if ((p.time_recorded - prev)/60 >= interval_in_minutes) {
			++n_samples;
			twap = (twap + p.price) / (twap > 0 ? 2 : 1);
			//                        ^^^^^^^^^^^^^^^^^^
			//                        so as to not include 0 as one of the prices
		}
		prev = p.time_recorded;
	}
	uint32_t n_expected_samples = (end_time - begin_time) / (interval_in_minutes * 60);
	if (n_samples < n_expected_samples) {
		throw std::runtime_error{"Not enough price samples were given to calculate TWAP over the specified time period and intervals"};
	}
	return twap;
}


/// Fetch price feed from Gemini
std::vector<timestamped_price_t> prices_from_gemini() {
	// TODO(zds): integrate asio for non-blocking http request
	fetch("https://api.gemini.com/v2/ticker/btcusd");
	return {};
}


// Fetch price feed from BitMEX
std::vector<timestamped_price_t> prices_from_bitmex() {
	// TODO(zds): integrate asio for non-blocking http requests
	// TODO(zds): integrate BitMEX's REST API
	return {};
}

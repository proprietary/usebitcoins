#ifndef _USEBITCOINS_PRICE_ORACLE_HPP
#define _USEBITCOINS_PRICE_ORACLE_HPP

#include <chrono>
#include <cstdint>
#include <string>
#include <vector>

namespace usebitcoins
{
namespace price_oracle
{

/// Price is encoded in cents of local currency e.g. USD
using fiat_price_t = uint64_t;

/// ISO-4217 currency codes
/// https://en.wikipedia.org/wiki/ISO_4217
enum class fiat_currency_t {
    USD,
    EUR,
    CNY,
    BRL,
    MXN,
    INR,
    JPY,
    RUB,
    // TODO: support common fiat currencies
};

class price_oracle_t
{
protected:
    fiat_currency_t base_currency_ = fiat_currency_t::USD;

public:
    /// volume-weighted average price over the past 24 hours
    // virtual fiat_price_t vwap_24_hours() const = 0;

    /// current price at this instant
    virtual fiat_price_t price() const = 0;

    void set_base_currency(fiat_currency_t base_currency)
    {
	base_currency_ = base_currency;
    }
};

class bitstamp_price_oracle_t : public price_oracle_t
{
public:
    // fiat_price_t vwap_24_hours() const override;
    fiat_price_t price() const override;
};

class gemini_price_oracle_t : public price_oracle_t
{
public:
    // fiat_price_t vwap_24_hours() const override;
    fiat_price_t price() const override;
};

class coinbase_price_oracle_t : public price_oracle_t
{
private:
    // used to self-rate limit
    inline static std::chrono::time_point<std::chrono::system_clock> last_request_time_ {};
    static void update_last_request_time();

public:
    static bool throttled();
    fiat_price_t price() const override;
};

} // namespace price_oracle
} // namespace usebitcoins

#endif // _USEBITCOINS_PRICE_ORACLE_HPP

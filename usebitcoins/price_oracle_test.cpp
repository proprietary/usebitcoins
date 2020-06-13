#include <gtest/gtest.h>
#include <price_oracle.hpp>
#include <iostream>

TEST(PriceOraclesTest, TestCoinbasePriceOracle) {
    using namespace usebitcoins::price_oracle;
    coinbase_price_oracle_t coinbase_oracle {};
    auto price = coinbase_oracle.price();
    std::cerr << "Bitcoin price (Coinbase): " << price << std::endl;
    EXPECT_GT(price, 0);
}

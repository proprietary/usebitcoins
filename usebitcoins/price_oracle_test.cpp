#include <gtest/gtest.h>
#include <price_oracle.hpp>
#include <iostream>

using namespace usebitcoins::price_oracle;

TEST(PriceOraclesTest, TestCoinbasePriceOracle) {
    coinbase_price_oracle_t coinbase_oracle {};
    auto price = coinbase_oracle.price();
    std::cerr << "Bitcoin price (Coinbase): " << price << std::endl;
    EXPECT_GT(price, 0);
}

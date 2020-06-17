#include <genaddress.hpp>
#include <gtest/gtest.h>
#include <iostream>

TEST(TestPrefilledAddressGenerator, TestChoosingAddress) {
    using namespace usebitcoins::genaddress;
    prefilled_address_generator_t a { {"bc1address1", "bc1address2"} };
    auto assigned_addr = a.gen_pub_addr(address_request_t{"mail@example.com"});
    EXPECT_GT(assigned_addr.size(), 0);
    EXPECT_STREQ(assigned_addr.c_str(), "bc1address1");
    auto assigned_addr_2 = a.gen_pub_addr(address_request_t{"hello@world.com"});
    EXPECT_STREQ(assigned_addr_2.c_str(), "bc1address2");
}

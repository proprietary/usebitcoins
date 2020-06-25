#include <gtest/gtest.h>
#include <iostream>
#include "usebitcoins/electrumx_rpc.hpp"

namespace
{
TEST(TestElectrumRPC, TestEstimateFee) {
    using namespace usebitcoins::electrumx_rpc;
    electrumx_server_params_t params {
	.hostname = "fortress.qtornado.com",
	.port = "443",
    };
    auto fee = estimate_fee(params, 50);
    std::cout << "fee: " << fee << std::endl;
    ASSERT_GE(fee, 1);
    auto fee_6 = estimate_fee(params, 6);
    std::cout << "fee (6 confs): " << fee_6 << std::endl;
    ASSERT_GE(fee_6, 1);
}
} // namespace


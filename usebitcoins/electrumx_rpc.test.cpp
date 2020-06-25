#include <gtest/gtest.h>
#include <iostream>
#include <electrumx_rpc.hpp>

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
}
} // namespace


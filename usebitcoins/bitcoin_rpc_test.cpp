#include <bitcoin_rpc.hpp>
#include <gtest/gtest.h>

// TODO: move all this to an integration test directory

class BitcoinRPCTest : public ::testing::Test
{
protected:
    usebitcoins::bitcoin_rpc::fullnode_params_t params;

    void SetUp() override
    {
	using usebitcoins::bitcoin_rpc::fullnode_params_t;
	// requires a fullnode running on current machine with this user:pwd rpc
	// params in ~/.bitcoin/bitcoin.conf
	// TODO: use a more robust test
	params = fullnode_params_t{
		.user = "user",
		.password = "badpassword",
		.host = "127.0.0.1",
		.port = 8332,
	};
    }
};

TEST_F(BitcoinRPCTest, TestEstimateFee)
{
    using usebitcoins::bitcoin_rpc::estimate_fee;
    using usebitcoins::bitcoin_rpc::fullnode_params_t;
    EXPECT_GE(estimate_fee(params, 6), 1);
    EXPECT_GE(estimate_fee(params, 20), 1);
    // std::cout << "estimate_fee 100 == " << estimate_fee(params, 100) << std::endl
    // 	      << "estimate_fee 6 == " << estimate_fee(params, 6) << std::endl
    // 	      << "estimate_fee 1 == " << estimate_fee(params, 1) << std::endl
    // 	      << "estimate_fee 2 == " << estimate_fee(params, 2) << std::endl
    // 	      << "estimate_fee 3 == " << estimate_fee(params, 3) << std::endl
    // 	      << "estimate_fee 20 == " << estimate_fee(params, 20) << std::endl
    // 	      << "estimate_fee 10 == " << estimate_fee(params, 10) << std::endl;
}

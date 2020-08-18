#include <hdkeygen.hpp>
#include <gtest/gtest.h>
#include <memory.h>

namespace
{

TEST(HDKeyDerivation, ParseStringDerivationPath) {
	std::string input {"44'/1'/1'/0"};
	auto res = hdkeys::parse_derivation_path(input);
	ASSERT_EQ(res.size(), 4);
	ASSERT_EQ(res[0].is_hardened, true);
	ASSERT_EQ(res[0].number, 44);
	ASSERT_EQ(res[1].is_hardened, true);
	ASSERT_EQ(res[1].number, 1);
	ASSERT_EQ(res[2].is_hardened, true);
	ASSERT_EQ(res[2].number, 1);
	ASSERT_EQ(res[3].is_hardened, false);
	ASSERT_EQ(res[3].number, 0);
}

class ExtendedPublicKeyDerivationTest : public ::testing::Test {
protected:
	std::unique_ptr<hdkeys::hdkeygen_t> hdkeygen_;
	
	void SetUp() override {
		hdkeygen_ = std::make_unique<hdkeys::hdkeygen_t>("xpub661MyMwAqRbcFtXgS5sYJABqqG9YLmC4Q1Rdap9gSE8NqtwybGhePY2gZ29ESFjqJoCu1Rupje8YtGqsefD265TMg7usUDFdp6W1EGMcet8");
	}
};

TEST_F(ExtendedPublicKeyDerivationTest, TestGeneration) {
	std::string const expected {"13Q3u97PKtyERBpXg31MLoJbQsECgJiMMw"};
	std::string const path {"0/1"};
	auto output = hdkeygen_->derive(path);
	// ::puts(output.c_str());
	// ::puts(expected.c_str());
	ASSERT_STREQ(output.c_str(), expected.c_str());
}


} // namespace

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}


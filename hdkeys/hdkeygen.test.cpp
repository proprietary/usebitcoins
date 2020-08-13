#include <hdkeygen.hpp>
#include <gtest/gtest.h>

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

} // namespace

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}


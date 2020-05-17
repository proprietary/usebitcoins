#include <gtest/gtest.h>
#include <hash.hpp>
#include <ios>
#include <iostream>
#include <memory>
#include <sodium.h>
#include <sstream>
#include <stdlib.h>

TEST(HashTest, HashEmailTest) {
	auto email_hash = hash::hash_email("mail@example.com", 100);
	// std::cout << email_hash.size() << std::endl;
	// for (auto ch : email_hash) {
	//	::printf("%X ", ch);
	// }
	// std::cout << std::endl;
	ASSERT_GT(email_hash.size(), 1);
}

TEST(HashTest, PackIntoBignumTest) {
	using namespace boost::multiprecision;
	auto email_hash = hash::hash_email("mail@example.com", 32);
	auto res = hash::pack_byte_array(email_hash);
	ASSERT_TRUE(res > 0);
	std::ostringstream a{};
	char buf[3];
	for (auto it = email_hash.rbegin(); it != email_hash.rend(); ++it) {
		::snprintf(&buf[0], 3, "%.2X", *it);
		a << buf[0] << buf[1];
	}
	std::ostringstream b{};
	b << std::hex << res;
	// std::cout << a.str() << std::endl << b.str() << std::endl;
	ASSERT_TRUE(a.str() == b.str());
	ASSERT_STREQ(b.str().c_str(),
		     "DBEE7CE79BF5FD700297C5BFB042B2B5C258EBB3DD520E15E47BB8898AA73E1C");
	ASSERT_STREQ(a.str().c_str(),
		     "DBEE7CE79BF5FD700297C5BFB042B2B5C258EBB3DD520E15E47BB8898AA73E1C");
}

int main(int argc, char *argv[]) {
	if (::sodium_init() < 0) {
		return 1;
	}
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

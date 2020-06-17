#include <gtest/gtest.h>
#include <hash.hpp>
#include <ios>
#include <iostream>
#include <memory>
#include <sodium.h>
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <string>
#include <random>

namespace {

double shannon_entropy(const uint8_t* const data, size_t n) {
    double entropy = 0.0;
    for (auto it = data; it != data + n; ++it) {
	auto count = static_cast<double>(std::count(data, data + n, *it));
	auto px = count / n;
	if (px > 0) {
	    entropy += -px * std::log2(px);
	}
    }
    return entropy;
}


TEST(HashEmail, HashEmailToIntegerTest) {
    using usebitcoins::hash::hash_email;
    EXPECT_GT(hash_email("mail@example.com"), 0UL);
    EXPECT_EQ(hash_email("mail@example.com"), hash_email("mail@example.com"));
    EXPECT_NE(hash_email("mail@example.com"), hash_email("noreply@example.com"));
    EXPECT_GT(hash_email("lol"), 0UL);
}

TEST(HashEmail, HashEmailToIntegerRandomnessTest) {
    using usebitcoins::hash::hash_email;
    using usebitcoins::hash::hash_string;
    auto hs = hash_string("mail@example.com");
    auto he = hash_email("mail@example.com");
    std::array<uint8_t, 8UL> he_bytes;
    std::memcpy(he_bytes.data(), &he, 8UL);
    auto string_entropy = shannon_entropy(hs.data(), hs.size());
    auto int_entropy= shannon_entropy(he_bytes.data(), 8);
    EXPECT_LT(string_entropy - int_entropy, 4);
}

TEST(HashEmail, HashEmailTestCollisions) {
    using usebitcoins::hash::hash_email;
    auto constexpr iters = 1'000'000'000;
    std::independent_bits_engine<std::default_random_engine, CHAR_BIT, uint8_t> rand_engine;
    auto const target = hash_email("mail@example.com");
    std::string rand_bytes;
    rand_bytes.resize(20);
    for (int i = 0; i < iters; ++i) {
	// generate random string of letters
	std::generate(rand_bytes.begin(), rand_bytes.end(), [&rand_engine]() {
	    return rand_engine() % ('z' - 'a') + 'a';
	});
	ASSERT_NE(hash_email(rand_bytes), target);
    }
}

int main(int argc, char *argv[]) {
	if (::sodium_init() < 0) {
		return 1;
	}
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

} // namespace

#include <algorithm>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/gmp.hpp>
#include <iterator>
#include <memory>
#include <sodium.h>
#include <stdexcept>
#include <stdint.h>
#include <string>
#include <vector>

namespace hash {

/// Fit a byte array into a bignum
/// Assumes that `src` is arranged in little-endian order
boost::multiprecision::uint256_t pack_byte_array(const std::vector<uint8_t> &src) {
    using namespace boost::multiprecision;
    size_t len = src.size();
    if (len % sizeof(limb_type)) {
	throw std::runtime_error{"length of byte array is not a multiple of the boost::multiprecision `limb_type`; the source byte array won't perfectly encode to the bignum"};
    }
    uint256_t out{};
    uint32_t sz = static_cast<uint32_t>(len)/sizeof(limb_type);
    out.backend().resize(sz, sz);
    ::memcpy(out.backend().limbs(), src.data(), src.size());
    out.backend().normalize();
    return out;
}

using email_address_t = std::string;

/// Perform straightforward hash of arbitrary data
std::vector<uint8_t> blake2_hash(std::vector<uint8_t> src) {
    // TODO wrap libsodium's implementation of BLAKE2
    // std::vector<uint8_t> out{crypto_generichash_BYTES, 0};
    std::unique_ptr<uint8_t[]> buf{new uint8_t[crypto_generichash_BYTES]};
    auto err =
	crypto_generichash(buf.get(), crypto_generichash_BYTES, src.data(), src.size(), nullptr, 0);
    if (err > 0) {
	throw std::runtime_error{""};
    }
    std::vector<uint8_t> out{};
    out.reserve(crypto_generichash_BYTES);
    std::copy(buf.get(), buf.get() + crypto_generichash_BYTES, std::back_inserter(out));
    return out;
}

/// Return at most the first N bytes of the hash of this email address encoded as a
/// UTF-8 string
std::vector<uint8_t> hash_email(const email_address_t &email_address, size_t n) {
    std::vector<uint8_t> email_address_bytes;
    std::copy(email_address.begin(), email_address.end(), std::back_inserter(email_address_bytes));
    auto hash = blake2_hash(email_address_bytes);
    size_t sz = std::min(n, hash.size());
    return std::vector<uint8_t>{hash.begin(), hash.begin() + sz};
}

} // namespace hash

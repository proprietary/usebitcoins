#include <algorithm>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/gmp.hpp>
#include <cstdlib>
#include <iterator>
#include <memory>
#include <sodium.h>
#include <span>
#include <stdexcept>
#include <stdint.h>
#include <string>
#include <vector>

namespace usebitcoins
{
namespace hash
{

namespace
{
uint64_t from_byte_array(const uint8_t* src, size_t n)
{
    n = std::max(0UL, std::min(8UL, n));
    uint64_t out = 0;
    for (int i = 0; i < n; ++i) {
    	out |= static_cast<uint64_t>(src[i] & 0xff) << (8 * i);
    }
    return out;
}

/// Condense a long hash into a u64 by repeatedly XORing its 8 byte pieces
uint64_t condense_hash(const std::vector<uint8_t>& byte_array)
{
    assert(byte_array.size() % 8 == 0);
    uint64_t out = 0;
    for (int i = 0; i < byte_array.size() / 8; ++i) {
	std::ptrdiff_t offset = i * 8;
	uint64_t chunk =
		from_byte_array(byte_array.data() + offset,
				std::min(8UL, byte_array.size() - offset));
	out ^= chunk;
    }
    return out;
}

} // namespace

/// Fit a byte array into a bignum
/// Assumes that `src` is arranged in little-endian order
boost::multiprecision::uint256_t
pack_byte_array(const std::vector<uint8_t>& src)
{
    using namespace boost::multiprecision;
    size_t len = src.size();
    if (len % sizeof(limb_type)) {
	throw std::runtime_error{
		"length of byte array is not a multiple of the "
		"boost::multiprecision `limb_type`; the source byte array "
		"won't perfectly encode to the bignum"};
    }
    uint256_t out{};
    uint32_t sz = static_cast<uint32_t>(len) / sizeof(limb_type);
    out.backend().resize(sz, sz);
    ::memcpy(out.backend().limbs(), src.data(), src.size());
    out.backend().normalize();
    return out;
}

using email_address_t = std::string;

/// Perform straightforward hash of arbitrary data
std::vector<uint8_t> blake2_hash(const std::vector<uint8_t>& src)
{
    // TODO wrap libsodium's implementation of BLAKE2
    // std::vector<uint8_t> out{crypto_generichash_BYTES, 0};
    std::unique_ptr<uint8_t[]> buf{new uint8_t[crypto_generichash_BYTES]};
    auto err = crypto_generichash(buf.get(), crypto_generichash_BYTES,
				  src.data(), src.size(), nullptr, 0);
    if (err > 0) {
	throw std::runtime_error{""};
    }
    std::vector<uint8_t> out{};
    out.reserve(crypto_generichash_BYTES);
    std::copy(buf.get(), buf.get() + crypto_generichash_BYTES,
	      std::back_inserter(out));
    return out;
}

std::vector<uint8_t> hash_string(std::string_view src) {
    std::vector<uint8_t> bytes;
    std::copy(src.begin(), src.end(), std::back_inserter(bytes));
    return bytes;
}

uint64_t hash_email(const email_address_t& email_address)
{
    auto email_address_bytes = hash_string(email_address);
    auto hash = blake2_hash(email_address_bytes);
    assert(hash.size() >= 32);
    uint64_t out = condense_hash(hash);
    return out;
}

} // namespace hash
} // namespace usebitcoins

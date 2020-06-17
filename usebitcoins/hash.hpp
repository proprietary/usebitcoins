#ifndef _HASH_HPP
#define _HASH_HPP
#include <string>
#include <vector>
#include <stdint.h>
#include <boost/multiprecision/gmp.hpp>
#include <boost/multiprecision/cpp_int.hpp>

namespace usebitcoins {
namespace hash {

// Generate BTC addresses deterministically by hashing the customer's ID
// (e.g., an email address)

using email_address_t = std::string;

/// Perform straightforward hash of arbitrary data
std::vector<uint8_t> blake2_hash(std::vector<uint8_t> src);

// Hash an arbitrary string via blake2 to a byte array
std::vector<uint8_t> hash_string(std::string_view src);

/// Hash email address to a 64-bit integer
uint64_t hash_email(const email_address_t& email_address);

/// Fit a byte array into a bignum
/// Assumes that `src` is arranged in little-endian order
boost::multiprecision::uint256_t pack_byte_array(const std::vector<uint8_t>& src);

} // namespace hash
} // namespace usebitcoins

#endif // _HASH_HPP

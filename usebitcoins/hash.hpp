#include <string>
#include <vector>
#include <stdint.h>
#include <boost/multiprecision/gmp.hpp>
#include <boost/multiprecision/cpp_int.hpp>


namespace hash {

// Generate BTC addresses deterministically by hashing the customer's ID
// (e.g., an email address)

using email_address_t = std::string;

/// Perform straightforward hash of arbitrary data
std::vector<uint8_t> blake2_hash(std::vector<uint8_t> src);

/// Return the first N bytes of the hash of this email address encoded as a UTF-8 string
std::vector<uint8_t> hash_email(const email_address_t& email_address, size_t n);

boost::multiprecision::uint256_t pack_byte_array(const std::vector<uint8_t>& src);

} // namespace hash

#include <string>
#include <vector>
#include <stdint.h>
#include <sodium.h>

namespace hash {

// Generate BTC addresses deterministically by hashing the customer's ID
// (e.g., an email address)

using email_address_t = std::string;


/// Perform straightforward hash of arbitrary data
std::vector<uint8_t> blake2_hash(std::vector<uint8_t> src) {
    // TODO wrap libsodium's implementation of BLAKE2
    return {};
}

/// Return the first N bytes of the hash of this email address encoded as a UTF-8 string
std::vector<uint8_t> hash_email(email_address_t email_address, size_t n) {
    return {};
}


const bitcoin_address_t generate_bitcoin_address(email_address_t email_address) {
    return {};
}

} // namespace hash

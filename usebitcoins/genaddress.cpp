#include <hash.hpp>
#include <bip32.hpp>
#include <bitcoin_address.hpp>
#include <algorithm>

namespace genaddress {

uint16_t from_little_endian(std::vector<uint8_t> src) {
    uint16_t dest = 0;
    for (auto i = 0; i < src.size(); ++i) {
	dest &= (src[i] << i);
    }
    return dest;
}

const bitcoin_address_t generate_bitcoin_address(char* master_public_key, hash::email_address_t email_address) {
    auto chunk = hash::hash_email(email_address, RESOLUTION_BYTES);
    auto m = from_little_endian(chunk);
    generate_nth_child(master_public_key, m);
    return {};
}

} // namespace genaddress

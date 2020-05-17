#include <genaddress.hpp>
#include <hash.hpp>
#include <algorithm>

namespace genaddress {

address_generator_t::bitcoin_public_address_t prefilled_address_generator_t::gen_pub_addr(address_request_t request) {
    auto h = hash::hash_email(request.email_address);
    auto n = hash::pack_byte_array(h);
    // compute an index into the public addresses
    auto index = n % public_addresses_.size();
    return public_addresses_[static_cast<decltype(public_addresses_.size())>(index)];
}

} // namespace genaddress

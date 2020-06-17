#include <genaddress.hpp>
#include <hash.hpp>
#include <algorithm>

namespace usebitcoins {
namespace genaddress {

address_generator_t::bitcoin_public_address_t prefilled_address_generator_t::gen_pub_addr(address_request_t request) {
    auto h = hash::hash_email(request.email_address);
    // compute an index into the public addresses
    auto index = h % public_addresses_.size();
    return public_addresses_[static_cast<decltype(public_addresses_.size())>(index)];
}

} // namespace genaddress
} // namespace usebitcoins

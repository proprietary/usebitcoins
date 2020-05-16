#ifndef GENADDRESS_HPP
#define GENADDRESS_HPP

#include <hash.hpp>
#include <bip32.hpp>
#include <bitcoin_address.hpp>

namespace genaddress {

const bitcoin_address_t generate_bitcoin_address(hash::email_address_t email_address);

/// Increasing this increases the uniqueness of address allocation to customers
static constexpr size_t RESOLUTION_BYTES = 2;

} // namespace genaddress

#endif // GENADDRESS_HPP

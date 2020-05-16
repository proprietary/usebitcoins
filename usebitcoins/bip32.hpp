#ifndef _BIP32_HPP
#define _BIP32_HPP

#include <vector>
#include <stdint.h>
#include <string>
#include <memory>
#include <bitcoin_address.hpp>

const bitcoin_address_t generate_nth_child(const std::vector<uint8_t>& master_public_key) noexcept;


#endif // _BIP32_HPP


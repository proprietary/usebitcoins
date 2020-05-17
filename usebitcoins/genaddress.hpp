#ifndef _GENADDRESS_HPP
#define _GENADDRESS_HPP

#include <boost/multiprecision/cpp_int.hpp>
#include <hash.hpp>
#include <string>
#include <vector>

namespace genaddress {

struct address_request_t {
	std::string email_address;
};

class address_generator_t {
      public:
	using bitcoin_public_address_t = std::string;
	virtual bitcoin_public_address_t gen_pub_addr(address_request_t) = 0;

      protected:
	virtual ~address_generator_t() {}
};

class prefilled_address_generator_t : public address_generator_t {
      private:
	std::vector<bitcoin_public_address_t> public_addresses_;

      public:
	prefilled_address_generator_t(std::vector<bitcoin_public_address_t> public_addresses)
	    : public_addresses_(public_addresses) {}
	bitcoin_public_address_t gen_pub_addr(address_request_t) final;
};

class bip32_address_generator_t : public address_generator_t {
      public:
	bip32_address_generator_t(std::vector<uint8_t> master_extended_pubkey);
	bitcoin_public_address_t gen_pub_addr(address_request_t) final;
};

/// Increasing this increases the uniqueness of address allocation to customers
static constexpr size_t RESOLUTION_BYTES = 2;

} // namespace genaddress

#endif // _GENADDRESS_HPP

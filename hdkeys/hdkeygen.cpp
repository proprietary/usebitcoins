#include <hdkeygen.hpp>
#include <key_io.h>
#include <key.h>
#include <pubkey.h>
#include <outputtype.h>
#include <stdexcept>
#include <tuple>
#include <array>
#include <algorithm>
#include <cstdint>
#include <cassert>
#include <string_view>
#include <charconv>
#include <utility>

namespace hdkeys
{

namespace
{

// auto hd_version(std::string const& extended_pubkey_string) -> std::tuple<chain_t, OutputType> {
// 	// https://github.com/satoshilabs/slips/blob/master/slip-0132.md
// 	std::array<char, 4> version_bytes;
// 	if (version_bytes.size() < 4) {
// 		throw std::runtime_error{"invalid extended public key"};
// 	}
// 	std::copy(extended_pubkey_string.begin(), extended_pubkey_string.begin() + 4, version_bytes.begin());
// 	chain_t chain;
// 	OutputType output_type;
// 	uint32_t magic_number = static_cast<uint32_t>(static_cast<uint8_t>(version_bytes[0]) << 24 |
// 												  static_cast<uint8_t>(version_bytes[1]) << 16 |
// 												  static_cast<uint8_t>(version_bytes[2]) << 8 |
// 												  static_cast<uint8_t>(version_bytes[3]));
// 	switch (magic_number) {
// 	case 0x0488b21e:
// 		// xpub
// 		// P2PKH or P2SH
// 		// m/44'/0'
// 		chain = chain_t::MAIN;
// 		output_type = OutputType::LEGACY;
// 		break;
// 	case 0x049d7cb2:
// 		// ypub
// 		// P2WPKH in P2SH
// 		// m/49'/0'
// 		chain = chain_t::MAIN;
// 		output_type = OutputType::P2SH_SEGWIT;
// 		break;
// 	case 0x04b27476:
// 		// zpub
// 		// P2WPKH
// 		// m/84'/0'
// 		chain = chain_t::MAIN;
// 		output_type = OutputType::BECH32;
// 		break;
// 	case 0x0295b43f:
// 		// Ypub
// 		// Multi-sig P2WSH in P2SH
// 		chain = chain_t::MAIN;
// 		output_type = OutputType::P2SH_SEGWIT;
// 		break;
// 	case 0x02aa7ed3:
// 		// Zpub
// 		// Multi-sig P2WSH
// 		chain = chain_t::MAIN;
// 		output_type = OutputType::BECH32;
// 		break;
// 	case 0x043587cf:
// 		// tpub
// 		// Testnet P2PKH or P2SH
// 		// m/44'/1'
// 		chain = chain_t::TEST;
// 		output_type = OutputType::LEGACY;
// 		break;
// 	case 0x044a5262:
// 		// upub
// 		// Testnet P2WPKH in P2SH
// 		// m/49'/1'
// 		chain = chain_t::TEST;
// 		output_type = OutputType::P2SH_SEGWIT;
// 		break;
// 	case 0x045f1cf6:
// 		// vpub
// 		// Testnet P2WPKH
// 		// m/84'/1'
// 		chain = chain_t::TEST;
// 		output_type = OutputType::BECH32;
// 		break;
// 	case 0x024289ef:
// 		// Upub
// 		// Testnet multi-sig P2WSH in P2SH
// 		chain = chain_t::TEST;
// 		output_type = OutputType::P2SH_SEGWIT;
// 		break;
// 	case 0x02575483:
// 		// Vpub
// 		// Testnet multi-sig P2WSH
// 		chain = chain_t::TEST;
// 		output_type = OutputType::BECH32;
// 		break;
// 	}
// 	return {chain, output_type};
// }


} // namespace


hdkeygen_t::hdkeygen_t(std::string const& extended_pubkey_string) { //bech32
	(void) ECCVerifyHandle();
	ECC_Start();
	// parse extended key string to get relevant information
	try {
		if (extended_pubkey_string.substr(1, 4) != "pub") {
			throw std::runtime_error{"invalid extended public key"};
		}
		// https://github.com/satoshilabs/slips/blob/master/slip-0132.md
		switch (extended_pubkey_string[0]) {
		case 'x':
			// P2PKH or P2SH
			chain_ = chain_t::MAIN;
			break;
		case 'y':
			// P2WPKH in P2SH
			chain_ = chain_t::MAIN;			
			break;
		case 'z':
			// P2WPKH
			chain_ = chain_t::MAIN;
			break;
		case 'Y':
			// multi-signature P2WSH in P2SH
			chain_ = chain_t::MAIN;
			break;
		case 'Z':
			// Multi-signature P2WSH
			chain_ = chain_t::MAIN;
			break;
		case 't':
			// Testnet P2PKH or P2SH
			chain_ = chain_t::TEST;
			break;
		case 'u':
			// Testnet P2WPKH in P2SH
			chain_ = chain_t::TEST;
			break;
		case 'v':
			// Testnet P2WPKH
			chain_ = chain_t::TEST;
			break;
		case 'U':
			// Multi-signature P2WSH in P2SH
			chain_ = chain_t::TEST;
			break;
		case 'V':
			// Multi-signature P2WSH
			chain_ = chain_t::TEST;
			break;
		default:
			assert(false);
		}
	} catch (std::out_of_range const& e) {
		throw std::runtime_error{"invalid extended public key"};
	}
	switch (chain_) {
	case chain_t::TEST:
		SelectParams("test");
		break;
	case chain_t::MAIN:
		SelectParams("main");
		break;
	}
	extended_pubkey_ = std::make_unique<CExtPubKey>(DecodeExtPubKey(extended_pubkey_string));
	// TODO: do the appropriate assignment based on the key type (see switch statement above)
	output_type_ = std::make_unique<OutputType>(OutputType::LEGACY);
}

hdkeygen_t::~hdkeygen_t() noexcept { ECC_Stop(); }

auto hdkeygen_t::derive(std::vector<hdkey_derivation_edge_t> const& derivation_path) -> std::string {
	CExtPubKey next_key = *extended_pubkey_;
	for (auto&& drv : derivation_path) {
		if (drv.is_hardened) {
			next_key.Derive(next_key, HARDENED_KEY_START + drv.number);
		} else {
			next_key.Derive(next_key, drv.number);
		}
	}
	auto dest = GetDestinationForKey(next_key.pubkey, *output_type_);
	return EncodeDestination(dest);
}

auto parse_derivation_path(std::string const& derivation_path)
	-> std::vector<hdkey_derivation_edge_t> {
	std::vector<hdkey_derivation_edge_t> out;
	std::string::size_type token_begin = 0, token_end = 0;
	std::string::size_type const len = derivation_path.size();
	do {
		hdkey_derivation_edge_t edge;
		token_end = derivation_path.find('/', token_begin);
		if (token_end == std::string::npos) {
			token_end = len;
		}
		auto part = derivation_path.substr(token_begin, token_end - token_begin);
		// consists of: [0-9]+[h']?
		// h or ' indicates hardened key
		std::string::size_type number_end = 0;
		for (; number_end < part.size() && part.at(number_end) >= '0' && part.at(number_end) <= '9'; ++number_end);
		auto number_part = part.substr(0, number_end);
		auto res = std::from_chars(number_part.data(), number_part.data() + number_part.size(), edge.number);
		if (res.ec == std::errc::invalid_argument) {
			return {};
		}
		if (number_end < part.size()) {
			auto hardened_indicator = part[number_end];
			edge.is_hardened = hardened_indicator == '\'' || hardened_indicator == 'h';
		} else {
			edge.is_hardened = false;
		}
		out.emplace_back(std::move(edge));
		token_begin = token_end + 1;
	} while (token_end != len);
	return out;
}

auto derive(std::string const& xpub, std::string const& derivation_path)
	-> std::string
{
	hdkeygen_t handle{xpub};
	auto path = parse_derivation_path(derivation_path);
	return handle.derive(path);
}

} // namespace

#ifndef _INCLUDE_HDKEYGEN_HPP
#define _INCLUDE_HDKEYGEN_HPP

#include <cstdint>
#include <string>
#include <vector>
#include <memory>

// forward-declare internal Bitcoin Core data structure
// these are hairy; avoid exposing to clients
class CExtPubKey;
enum class OutputType;
class ECCVerifyHandle;

namespace hdkeys
{

static uint64_t constexpr HARDENED_KEY_START = 0x80000000;

struct hdkey_derivation_edge_t {
    int number;
    bool is_hardened;
};

auto parse_derivation_path(std::string const& derivation_path)
	-> std::vector<hdkey_derivation_edge_t>;

auto derive(std::string const& xpub, std::string const& derivation_path)
	-> std::string;

enum class chain_t {
	MAIN,
	TEST,
} chain_;

class hdkeygen_t {
private:
	// pointers to hairy, internal Bitcoin Core data types so that they need not be in header for consumer
	std::unique_ptr<CExtPubKey> extended_pubkey_;
	std::unique_ptr<OutputType> output_type_;
	std::unique_ptr<ECCVerifyHandle> ecc_verify_handle_;
public:
	explicit hdkeygen_t(std::string const& extended_pubkey_string);

	~hdkeygen_t() noexcept;

	hdkeygen_t(hdkeygen_t&& src) noexcept;

	hdkeygen_t(hdkeygen_t const&);

	hdkeygen_t& operator=(hdkeygen_t const&);

	hdkeygen_t& operator=(hdkeygen_t&&) noexcept;

	auto derive(std::vector<hdkey_derivation_edge_t> const& derivation_path) -> std::string;

	auto derive(std::string const& derivation_path) -> std::string {
		auto derivation = parse_derivation_path(derivation_path);
		return derive(derivation);
	}
};

} // namespace hdkeys

#endif // _INCLUDE_HDKEYGEN_HPP

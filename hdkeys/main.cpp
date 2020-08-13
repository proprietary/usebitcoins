#include <pubkey.h>
#include <key_io.h>
#include <outputtype.h>
#include <string>
#include <iostream>
#include <secp256k1.h>
#include <secp256k1_preallocated.h>
#include <cassert>
#include <util/strencodings.h>
#include <key.h>
#include <hdkeygen.hpp>

int main() {
	using hdkeys::HARDENED_KEY_START;
	
	auto ecc_handle = ECCVerifyHandle();
	(void) ecc_handle;
	ECC_Start();

	SelectParams("main");

	std::string const xpub = "xpub661MyMwAqRbcFtXgS5sYJABqqG9YLmC4Q1Rdap9gSE8NqtwybGhePY2gZ29ESFjqJoCu1Rupje8YtGqsefD265TMg7usUDFdp6W1EGMcet8";
	std::string const xprv = "xprv9s21ZrQH143K3QTDL4LXw2F7HEK3wJUD2nW2nRk4stbPy6cq3jPPqjiChkVvvNKmPGJxWUtg6LnF5kejMRNNU3TGtRBeJgk33yuGBxrMPHi";
	std::string const seed = "000102030405060708090a0b0c0d0e0f";
	auto parsed_seed = ParseHex(seed);
	CExtKey key;
	key.SetSeed(parsed_seed.data(), parsed_seed.size());
	CExtKey keyNew;
	key.Derive(keyNew, HARDENED_KEY_START + 41);
	auto pubkey = keyNew.Neuter();
	auto dest = GetDestinationForKey(pubkey.pubkey, OutputType::LEGACY);
	std::cout << EncodeDestination(dest) << std::endl;

	ECC_Stop();
	return 0;
}

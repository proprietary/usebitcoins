#include <pubkey.h>
#include <key_io.h>
#include <outputtype.h>
#include <string>
#include <iostream>
#include <secp256k1.h>
#include <secp256k1_preallocated.h>
#include <cassert>

int main() {
	auto ecc_handle = ECCVerifyHandle();
	(void) ecc_handle;

	SelectParams("main");

	std::string const xpub = "xpub661MyMwAqRbcFtXgS5sYJABqqG9YLmC4Q1Rdap9gSE8NqtwybGhePY2gZ29ESFjqJoCu1Rupje8YtGqsefD265TMg7usUDFdp6W1EGMcet8";
	std::string const xprv = "xprv9s21ZrQH143K3QTDL4LXw2F7HEK3wJUD2nW2nRk4stbPy6cq3jPPqjiChkVvvNKmPGJxWUtg6LnF5kejMRNNU3TGtRBeJgk33yuGBxrMPHi";
	std::string const seed = "000102030405060708090a0b0c0d0e0f";
	CExtPubKey pubkey = DecodeExtPubKey(xpub);
	CExtPubKey pubkey1;
	pubkey.Derive(pubkey1, 1);
	std::cout << EncodeExtPubKey(pubkey1) << std::endl;
	CExtPubKey pubkey1_1;
	pubkey1.Derive(pubkey1_1, 1);
	std::cout << EncodeExtPubKey(pubkey1_1) << std::endl;
	CExtPubKey pubkey1_1_1;
	pubkey1_1.Derive(pubkey1_1_1, 1);
	std::cout << EncodeExtPubKey(pubkey1_1_1) << std::endl;
	auto txdest = GetDestinationForKey(pubkey1_1_1.pubkey, OutputType::LEGACY);
	std::cout << EncodeDestination(txdest) << std::endl;
	return 0;
}

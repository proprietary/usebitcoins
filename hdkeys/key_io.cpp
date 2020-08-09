// Copyright (c) 2014-2017 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <key_io.h>
#include <pubkey.h>

#include <base58.h>
// #include <bech32.h>
// #include <script/script.h>
// #include <utilstrencodings.h>

#include <assert.h>
#include <string.h>
#include <algorithm>

namespace {
enum Base58Type {
    PUBKEY_ADDRESS,
    SCRIPT_ADDRESS,
    SECRET_KEY,
    EXT_PUBLIC_KEY,
    EXT_SECRET_KEY,

    MAX_BASE58_TYPES
};

std::vector<unsigned char> mainBase58Prefixes[MAX_BASE58_TYPES];

void init() {
	mainBase58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,0);
	mainBase58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5);
	mainBase58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,128);
	mainBase58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
	mainBase58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};
}
} // namespace

CExtPubKey DecodeExtPubKey(const std::string& str)
{
	init(); //hack	
    CExtPubKey key;
    std::vector<unsigned char> data;
    if (DecodeBase58Check(str, data, 78)) {
        const std::vector<unsigned char>& prefix = mainBase58Prefixes[EXT_PUBLIC_KEY];
        if (data.size() == BIP32_EXTKEY_SIZE + prefix.size() && std::equal(prefix.begin(), prefix.end(), data.begin())) {
            key.Decode(data.data() + prefix.size());
        }
    }
    return key;
}

std::string EncodeExtPubKey(const CExtPubKey& key)
{
    std::vector<unsigned char> data = mainBase58Prefixes[EXT_PUBLIC_KEY];
    size_t size = data.size();
    data.resize(size + BIP32_EXTKEY_SIZE);
    key.Encode(data.data() + size);
    std::string ret = EncodeBase58Check(data);
    return ret;
}

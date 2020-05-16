// Copyright (c) 2014-2019 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <key_io.h>

#include <base58.h>
#include <bech32.h>
#include <util/strencodings.h>

#include <boost/variant/apply_visitor.hpp>
#include <boost/variant/static_visitor.hpp>

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

const std::vector<unsigned char> base58_prefix(Base58Type t) {
    switch (t) {
    case PUBKEY_ADDRESS:
	return std::vector<unsigned char>(1, 0);
    case SCRIPT_ADDRESS:
	return std::vector<unsigned char>(1, 5);
    case SECRET_KEY:
	return std::vector<unsigned char>(1, 128);
    case EXT_PUBLIC_KEY:
	return {0x04, 0x88, 0xB2, 0x1E};
    case EXT_SECRET_KEY:
	return {0x04, 0x88, 0xAD, 0xE4};
    default:
	return {};
    }
}
} // namespace


CExtPubKey DecodeExtPubKey(const std::string& str)
{
    CExtPubKey key;
    std::vector<unsigned char> data;
    if (DecodeBase58Check(str, data, 78)) {
        const std::vector<unsigned char>& prefix = base58_prefix(EXT_PUBLIC_KEY);
        if (data.size() == BIP32_EXTKEY_SIZE + prefix.size() && std::equal(prefix.begin(), prefix.end(), data.begin())) {
            key.Decode(data.data() + prefix.size());
        }
    }
    return key;
}

std::string EncodeExtPubKey(const CExtPubKey& key)
{
    std::vector<unsigned char> data = base58_prefix(EXT_PUBLIC_KEY);
    size_t size = data.size();
    data.resize(size + BIP32_EXTKEY_SIZE);
    key.Encode(data.data() + size);
    std::string ret = EncodeBase58Check(data);
    return ret;
}

CExtKey DecodeExtKey(const std::string& str)
{
    CExtKey key;
    std::vector<unsigned char> data;
    if (DecodeBase58Check(str, data, 78)) {
        const std::vector<unsigned char>& prefix = base58_prefix(EXT_SECRET_KEY);
        if (data.size() == BIP32_EXTKEY_SIZE + prefix.size() && std::equal(prefix.begin(), prefix.end(), data.begin())) {
            key.Decode(data.data() + prefix.size());
        }
    }
    return key;
}

std::string EncodeExtKey(const CExtKey& key)
{
    std::vector<unsigned char> data = base58_prefix(EXT_SECRET_KEY);
    size_t size = data.size();
    data.resize(size + BIP32_EXTKEY_SIZE);
    key.Encode(data.data() + size);
    std::string ret = EncodeBase58Check(data);
    memory_cleanse(data.data(), data.size());
    return ret;
}

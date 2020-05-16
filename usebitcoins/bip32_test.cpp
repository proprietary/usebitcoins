#include <string>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <gtest/gtest.h>

#include <key.h>
#include <key_io.h>
#include <strencodings.h>

class BIP32Test : public ::testing::Test {
public:
    ECCVerifyHandle global_verify_handle_;
public:
    void SetUp() override {
	ECC_Start();
    }
};

TEST_F(BIP32Test, TestFingerprint) {
    std::string str_hex_master { "000102030405060708090a0b0c0d0e0f" };
    std::vector<unsigned char> seed = ParseHex(str_hex_master);
    CExtKey key;
    CExtPubKey pubkey;
    key.SetSeed(seed.data(), seed.size());
    pubkey = key.Neuter(); // segfaults unless ECC_Start() is called before all this
    unsigned char data[74];
    key.Encode(data);
    pubkey.Encode(data);
    std::string pub {"xpub661MyMwAqRbcFtXgS5sYJABqqG9YLmC4Q1Rdap9gSE8NqtwybGhePY2gZ29ESFjqJoCu1Rupje8YtGqsefD265TMg7usUDFdp6W1EGMcet8"};
    std::string priv {"xprv9s21ZrQH143K3QTDL4LXw2F7HEK3wJUD2nW2nRk4stbPy6cq3jPPqjiChkVvvNKmPGJxWUtg6LnF5kejMRNNU3TGtRBeJgk33yuGBxrMPHi"};
    // test private key
    ASSERT_EQ(EncodeExtKey(key), priv);
    ASSERT_EQ(DecodeExtKey(priv), key);
    // test public key
    ASSERT_EQ(EncodeExtPubKey(pubkey), pub);
    ASSERT_EQ(DecodeExtPubKey(pub), pubkey);
    // derive new public key
    CExtKey new_key;
    auto n_child = 0x1;
    // derive from private key
    key.Derive(new_key, n_child);
    CExtPubKey new_pub_key;
    new_pub_key = new_key.Neuter();
    // compare with public derivation
    CExtPubKey new_pub_key_2;
    pubkey.Derive(new_pub_key_2, n_child);
    ASSERT_TRUE(new_pub_key_2 == new_pub_key);
    auto new_key_as_str = EncodeExtPubKey(new_pub_key);
    std::cout << new_key_as_str << std::endl;
}


int main (int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

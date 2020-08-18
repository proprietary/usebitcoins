#include <gtest/gtest.h>
#include <key.h>
#include <key_io.h>
#include <memory.h>
#include <outputtype.h>
#include <pubkey.h>
#include <string>
#include <util/strencodings.h>
#include <vector>

namespace
{

class TestBitcoinCoreBIP32 : public ::testing::Test
{
    protected:
	std::unique_ptr<ECCVerifyHandle> ecc_handle_;

	void SetUp() override
	{
		ecc_handle_ = std::make_unique<ECCVerifyHandle>();
		ECC_Start();
		SelectParams("main");
	}

	void TearDown() override { ECC_Stop(); }
};

struct TestDerivation {
	std::string pub;
	std::string prv;
	unsigned int nChild;
};

struct TestVector {
	std::string strHexMaster;
	std::vector<TestDerivation> vDerive;

	explicit TestVector(std::string strHexMasterIn) : strHexMaster(strHexMasterIn) {}

	TestVector& operator()(std::string pub, std::string prv, unsigned int nChild)
	{
		vDerive.push_back(TestDerivation());
		TestDerivation& der = vDerive.back();
		der.pub = pub;
		der.prv = prv;
		der.nChild = nChild;
		return *this;
	}
};

TestVector test1 = TestVector("000102030405060708090a0b0c0d0e0f")(
	"xpub661MyMwAqRbcFtXgS5sYJABqqG9YLmC4Q1Rdap9gSE8NqtwybGhePY2gZ29ESFjqJo"
	"Cu1Rupje8YtGqsefD265TMg7usUDFdp6W1EGMcet8",
	"xprv9s21ZrQH143K3QTDL4LXw2F7HEK3wJUD2nW2nRk4stbPy6cq3jPPqjiChkVvvNKmPG"
	"JxWUtg6LnF5kejMRNNU3TGtRBeJgk33yuGBxrMPHi",
	0x80000000)("xpub68Gmy5EdvgibQVfPdqkBBCHxA5htiqg55crXYuXoQRKfDBFA1WEjWg"
		    "P6LHhwBZeNK1VTsfTFUHCdrfp1bgwQ9xv5ski8PX9rL2dZXvgGDnw",
		    "xprv9uHRZZhk6KAJC1avXpDAp4MDc3sQKNxDiPvvkX8Br5ngLNv1TxvUxt"
		    "4cV1rGL5hj6KCesnDYUhd7oWgT11eZG7XnxHrnYeSvkzY7d2bhkJ7",
		    1)("xpub6ASuArnXKPbfEwhqN6e3mwBcDTgzisQN1wXN9BJcM47sSikHjJf3UFHKkNAWbWMiGj"
		       "7Wf5uMash7SyYq527Hqck2AxYysAA7xmALppuCkwQ",
		       "xprv9wTYmMFdV23N2TdNG573QoEsfRrWKQgWeibmLntzniatZvR9BmLnvSxqu53Kw1UmYP"
		       "xLgboyZQaXwTCg8MSY3H2EU4pWcQDnRnrVA1xe8fs",
		       0x80000002)("xpub6D4BDPcP2GT577Vvch3R8wDkScZWzQzMMUm3PWbmWvVJrZwQY4VUNg"
				   "qFJPMM3No2dFDFGTsxxpG5uJh7n7epu4trkrX7x7DogT5Uv6fcLW5",
				   "xprv9z4pot5VBttmtdRTWfWQmoH1taj2axGVzFqSb8C9xaxKymcFzXBDpt"
				   "WmT7FwuEzG3ryjH4ktypQSAewRiNMjANTtpgP4mLTj34bhnZX7UiM",
				   2)(
	"xpub6FHa3pjLCk84BayeJxFW2SP4XRrFd1JYnxeLeU8EqN3vDfZmbqBqaGJAyiLjTAwm6Z"
	"LRQUMv1ZACTj37sR62cfN7fe5JnJ7dh8zL4fiyLHV",
	"xprvA2JDeKCSNNZky6uBCviVfJSKyQ1mDYahRjijr5idH2WwLsEd4Hsb2Tyh8RfQMuPh7f"
	"7RtyzTtdrbdqqsunu5Mm3wDvUAKRHSC34sJ7in334",
	1000000000)("xpub6H1LXWLaKsWFhvm6RVpEL9P4KfRZSW7abD2ttkWP3SSQvnyA8FSVqN"
		    "TEcYFgJS2UaFcxupHiYkro49S8yGasTvXEYBVPamhGW6cFJodrTHy",
		    "xprvA41z7zogVVwxVSgdKUHDy1SKmdb533PjDz7J6N6mV6uS3ze1ai8FHa"
		    "8kmHScGpWmj4WggLyQjgPie1rFSruoUihUZREPSL39UNdE3BBDu76",
		    0);

TestVector test2 = TestVector(
	"fffcf9f6f3f0edeae7e4e1dedbd8d5d2cfccc9c6c3c0bdbab7b4b1aeaba8a5a29f9c99"
	"9693908d8a8784817e7b7875726f6c696663605d5a5754514e4b484542")(
	"xpub661MyMwAqRbcFW31YEwpkMuc5THy2PSt5bDMsktWQcFF8syAmRUapSCGu8ED9W6oDM"
	"Sgv6Zz8idoc4a6mr8BDzTJY47LJhkJ8UB7WEGuduB",
	"xprv9s21ZrQH143K31xYSDQpPDxsXRTUcvj2iNHm5NUtrGiGG5e2DtALGdso3pGz6ssrdK"
	"4PFmM8NSpSBHNqPqm55Qn3LqFtT2emdEXVYsCzC2U",
	0)("xpub69H7F5d8KSRgmmdJg2KhpAK8SR3DjMwAdkxj3ZuxV27CprR9LgpeyGmXUbC6wb7"
	   "ERfvrnKZjXoUmmDznezpbZb7ap6r1D3tgFxHmwMkQTPH",
	   "xprv9vHkqa6EV4sPZHYqZznhT2NPtPCjKuDKGY38FBWLvgaDx45zo9WQRUT3dKYnjwi"
	   "h2yJD9mkrocEZXo1ex8G81dwSM1fwqWpWkeS3v86pgKt",
	   0xFFFFFFFF)("xpub6ASAVgeehLbnwdqV6UKMHVzgqAG8Gr6riv3Fxxpj8ksbH9ebxaEyBLZ85ySDhKiLDB"
		       "rQSARLq1uNRts8RuJiHjaDMBU4Zn9h8LZNnBC5y4a",
		       "xprv9wSp6B7kry3Vj9m1zSnLvN3xH8RdsPP1Mh7fAaR7aRLcQMKTR2vidYEeEg2mUCTAwC"
		       "d6vnxVrcjfy2kRgVsFawNzmjuHc2YmYRmagcEPdU9",
		       1)("xpub6DF8uhdarytz3FWdA8TvFSvvAh8dP3283MY7p2V4SeE2wyWmG5mg5EwVvmdMVCQ"
			  "coNJxGoWaU9DCWh89LojfZ537wTfunKau47EL2dhHKon",
			  "xprv9zFnWC6h2cLgpmSA46vutJzBcfJ8yaJGg8cX1e5StJh45BBciYTRXSd25UEPVue"
			  "sF9yog62tGAQtHjXajPPdbRCHuWS6T8XA2ECKADdw4Ef",
			  0xFFFFFFFE)(
	"xpub6ERApfZwUNrhLCkDtcHTcxd75RbzS1ed54G1LkBUHQVHQKqhMkhgbmJbZRkrgZw4ko"
	"xb5JaHWkY4ALHY2grBGRjaDMzQLcgJvLJuZZvRcEL",
	"xprvA1RpRA33e1JQ7ifknakTFpgNXPmW2YvmhqLQYMmrj4xJXXWYpDPS3xz7iAxn8L39nj"
	"GVyuoseXzU6rcxFLJ8HFsTjSyQbLYnMpCqE2VbFWc",
	2)("xpub6FnCn6nSzZAw5Tw7cgR9bi15UV96gLZhjDstkXXxvCLsUXBGXPdSnLFbdpq8p9H"
	   "mGsApME5hQTZ3emM2rnY5agb9rXpVGyy3bdW6EEgAtqt",
	   "xprvA2nrNbFZABcdryreWet9Ea4LvTJcGsqrMzxHx98MMrotbir7yrKCEXw7nadnHM8"
	   "Dq38EGfSh6dqA9QWTyefMLEcBYJUuekgW4BYPJcr9E7j",
	   0);

TestVector test3 =
	TestVector("4b381541583be4423346c643850da4b320e46a87ae3d2a4e6da11eba819cd4acba45d2"
		   "39319ac14f863b8d5ab5a0d0c64d2e8a1e7d1457df2e5a3c51c73235be")(
		"xpub661MyMwAqRbcEZVB4dScxMAdx6d4nFc9nvyvH3v4gJL378CSRZiYmhRoP7mBy6gSPS"
		"CYk6SzXPTf3ND1cZAceL7SfJ1Z3GC8vBgp2epUt13",
		"xprv9s21ZrQH143K25QhxbucbDDuQ4naNntJRi4KUfWT7xo4EKsHt2QJDu7KXp1A3u7Bi1"
		"j8ph3EGsZ9Xvz9dGuVrtHHs7pXeTzjuxBrCmmhgC6",
		0x80000000)("xpub68NZiKmJWnxxS6aaHmn81bvJeTESw724CRDs6HbuccFQN9Ku14VQrA"
			    "DWgqbhhTHBaohPX4CjNLf9fq9MYo6oDaPPLPxSb7gwQN3ih19Zm4Y",
			    "xprv9uPDJpEQgRQfDcW7BkF7eTya6RPxXeJCqCJGHuCJ4GiRVLzkTXBAJM"
			    "u2qaMWPrS7AANYqdq6vcBcBUdJCVVFceUvJFjaPdGZ2y9WACViL4L",
			    0);

TEST_F(TestBitcoinCoreBIP32, TestVectors)
{
	for (auto& test : std::vector<TestVector>{{test1, test2, test3}}) {
		std::vector<unsigned char> seed = ParseHex(test.strHexMaster);
		CExtKey key;
		CExtPubKey pubkey;
		key.SetSeed(seed.data(), seed.size());
		pubkey = key.Neuter();
		for (const TestDerivation& derive : test.vDerive) {
			unsigned char data[74];
			key.Encode(data);
			pubkey.Encode(data);

			// Test private key
			// ASSERT_TRUE(EncodeExtKey(key) == derive.prv);
			// ASSERT_TRUE(DecodeExtKey(derive.prv) == key);
			// //ensure a base58 decoded key also matches

			// Test public key
			ASSERT_TRUE(EncodeExtPubKey(pubkey) == derive.pub);
			ASSERT_TRUE(DecodeExtPubKey(derive.pub) ==
				    pubkey); // ensure a base58 decoded pubkey
					     // also matches

			// Derive new keys
			CExtKey keyNew;
			ASSERT_TRUE(key.Derive(keyNew, derive.nChild));
			CExtPubKey pubkeyNew = keyNew.Neuter();
			if (!(derive.nChild & 0x80000000)) {
				// Compare with public derivation
				CExtPubKey pubkeyNew2;
				ASSERT_TRUE(pubkey.Derive(pubkeyNew2, derive.nChild));
				ASSERT_TRUE(pubkeyNew == pubkeyNew2);
			}
			key = keyNew;
			pubkey = pubkeyNew;
		}
	}
}

} // namespace

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

#include <vector>
#include <string>
#include <bip32.hpp>
#include <cassert>
// #include <key.h>
// #include <key_io.h>
// #include <util/strencodings.h>
// 
// 
// int main(int argc, char* argv[]) {
//     std::vector<unsigned char> seed = ParseHex("000102030405060708090a0b0c0d0e0f");
//     CExtKey key;
//     CExtPubKey pubkey;
//     key.SetSeed(seed.data(), seed.size());
//     pubkey = key.Neuter();
//     return 0;
// }


#include <cstdlib>
#include <string>
#include <bitcoin/system.hpp>

BC_USE_LIBBITCOIN_MAIN

// Testing out our http://utf8everywhere.org implementation.
int bc::system::main(int argc, char* argv[])
{
    using namespace bc;
    using namespace bc::system;

    // Windows utf8 everywhere demonstration.
    set_utf8_stdio();

    system::cout << "output : acción.кошка.日本国" << std::endl;
    system::cerr << "error : acción.кошка.日本国" << std::endl;

    system::cout << "Enter text to input..." << std::endl;
    std::string console;
    system::cin >> console;
    system::cout << "input[0]  : " << console << std::endl;

    if (argc > 1)
        system::cout << "argv[1] : " << argv[1] << std::endl;

#ifdef _MSC_VER
    if (environ[0] != nullptr)
        system::cout << "environ[0] : " << environ[0] << std::endl;
#endif

    // Extracting Satoshi's words from genesis block.
    const chain::block block = settings(config::settings::mainnet)
        .genesis_block;
    const auto& coinbase = block.transactions().front();
    const auto& input = coinbase.inputs().front();
    BITCOIN_ASSERT_MSG(input.script().size() > 2u, "unexpected genesis");

    const auto headline = input.script()[2].data();
    std::string message(headline.begin(), headline.end());
    system::cout << message << std::endl;

    return EXIT_SUCCESS;
}

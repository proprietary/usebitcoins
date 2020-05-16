package(default_visibility = ["//visibility:public"])

cc_library(
    name = "bitcoin_lib",
    srcs = glob([
        "src/pubkey.cpp",
        "src/key.cpp",
        "src/uint256.cpp",
        "src/util/strencodings.cpp",
        "src/util/string.cpp",
        "src/hash.cpp",
        "src/random.cpp",
        "src/logging.cpp",
        "src/randomenv.cpp",
        "src/crypto/*.cpp",
        "src/support/*.cpp",
        "src/support/allocators/*.cpp",
    ]),
    hdrs = glob([
        "src/*.h",
        "src/util/*.h",
        "src/compat/*.h",
        "src/crypto/*.h",
        "src/crypto/ctaes/*.h",
        "src/crypto/ctaes/*.c",
        "src/support/*.h",
        "src/support/allocators/*.h",
        "src/conifg/*.h",
    ]),
    textual_hdrs = glob([
        "src/crypto/*.h",
        "src/crypto/*.c",
        "src/config/*.h",
    ]),
    local_defines = [
        "CLIENT_VERSION_BUILD 0",
        "CLIENT_VERSION_MAJOR 0",
        "CLIENT_VERSION_MINOR 20",
        "CLIENT_VERSION_REVISION 99",
    ],
    includes = [
        "src",
        "src/crypto/ctaes",
    ],
    linkopts = [
        "-l:/usr/local/lib/libsecp256k1.so",
    ],
    deps = [
        #"@com_github_bitcoin-core_secp256k1//:secp256k1",
    ],
    strip_include_prefix = "src",
    visibility = ["//visibility:public"],
)

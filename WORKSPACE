load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository", "new_git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# rules for building autotools and CMake projects

# Group the sources of the library so that CMake rule have access to it
all_content = """filegroup(name = "all", srcs = glob(["**"]), visibility = ["//visibility:public"])"""

# Rule repository
http_archive(
   name = "rules_foreign_cc",
   strip_prefix = "rules_foreign_cc-master",
   url = "https://github.com/bazelbuild/rules_foreign_cc/archive/master.zip",
)

load("@rules_foreign_cc//:workspace_definitions.bzl", "rules_foreign_cc_dependencies")

# Call this function from the WORKSPACE file to initialize rules_foreign_cc
#  dependencies and let neccesary code generation happen
#  (Code generation is needed to support different variants of the C++ Starlark API.).
#
#  Args:
#    native_tools_toolchains: pass the toolchains for toolchain types
#      '@rules_foreign_cc//tools/build_defs:cmake_toolchain' and
#      '@rules_foreign_cc//tools/build_defs:ninja_toolchain' with the needed platform constraints.
#      If you do not pass anything, registered default toolchains will be selected (see below).
#  
#    register_default_tools: if True, the cmake and ninja toolchains, calling corresponding
#      preinstalled binaries by name (cmake, ninja) will be registered after
#      'native_tools_toolchains' without any platform constraints.
#      The default is True.
rules_foreign_cc_dependencies([])


# rules_boost -- Bazel build rules for Boost

git_repository(
    name = "com_github_nelhage_rules_boost",
    commit = "9f9fb8b2f0213989247c9d5c0e814a8451d18d7f",
    remote = "https://github.com/nelhage/rules_boost",
    shallow_since = "1570056263 -0700",
)

load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")
boost_deps()

# Bitcoin

#new_git_repository(
#    name = "com_github_bitcoin_bitcoin",
#    commit = "04c09553d89809cf6328679d7535ecaa0070485d",
#    remote = "https://github.com/bitcoin/bitcoin",
#    build_file = "//:third_party/bitcoin.BUILD",
#    # TODO shallow_since = "",
#)

new_local_repository(
    name = "com_github_bitcoin_bitcoin",
    path = "third_party/bitcoin",
    build_file = "//:third_party/bitcoin.BUILD",
)

new_local_repository(
    name = "secp256k1",
    path = "third_party/secp256k1",
    build_file = "//:third_party/secp256k1.BUILD",
)

new_local_repository(
    name = "com_github_libbtc_libbtc",
    path = "third_party/libbtc",
    build_file = "//:third_party/libbtc.BUILD",
)

# Libsodium

http_archive(
    name = "org_libsodium_libsodium",
    build_file = "//:third_party/libsodium.BUILD",
    url = "https://download.libsodium.org/libsodium/releases/libsodium-1.0.18-stable.tar.gz",
    sha256 = "be151ef8a287cc335469575e53432fc42054aecf2fe5a0e910f2b7cbb6e001ac",
    patch_cmds = ["./autogen.sh"],
    strip_prefix = "libsodium-stable",
)

# Googletest

# http_archive(
#     name = "gtest",
#     url = "https://github.com/google/googletest/archive/release-1.7.0.zip",
#     sha256 = "b58cb7547a28b2c718d1e38aee18a3659c9e3ff52440297e965f5edffe34b6d0",
#     build_file = "@//:gtest.BUILD",
# )

git_repository(
    name = "com_github_google_googletest",
    remote = "https://github.com/google/googletest.git",
    commit = "011959aafddcd30611003de96cfd8d7a7685c700",
    # TODO shallow_since = "",
)


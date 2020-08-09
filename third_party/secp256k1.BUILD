package(default_visibility = ["//visibility:public"])

# load("@rules_foreign_cc//tools/build_defs:configure.bzl", "configure_make")

# filegroup(
#     name = "all",
#     srcs = glob(["**"]),
#     visibility = ["//visibility:public"],
# )

# configure_make(
#     name = "secp256k1",
#     configure_options = [
#         "--enable-module-recovery",
#     ],
#     lib_source = "@secp256k1//:all",
#     static_libraries = [
#         "libsecp256k1.a",
#     ],
# )

cc_library(
    name = "secp256k1",
    srcs = glob([
        "src/secp256k1.c",
        "src/gen_context.c",
        "src/**/*.h",
        "contrib/*.h",
        "contrib/*.c",
    ], exclude=[
        "src/modules/ecdh/*",
    ]),
    local_defines = [
        "USE_NUM_NONE=1",
        "USE_FIELD_10X26=1",
        "USE_SCALAR_INV_BUILTIN=1",
        "USE_FIELD_INV_BUILTIN=1",
        "USE_SCALAR_8X32=1",
        "ECMULT_WINDOW_SIZE=15",
        "ECMULT_GEN_PREC_BITS=8",
    ],
    hdrs = glob([
        "src/*.h",
        "src/modules/recovery/main_impl.h",
        "include/*.h",
    ]),
    includes = [
        "include",
        "contrib",
        "src",
    ],
    linkopts = [],
)

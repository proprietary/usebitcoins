package(default_visibility = ["//visibility:public"])

load("@rules_foreign_cc//tools/build_defs:configure.bzl", "configure_make")

genrule(
    name = "autogen_sh",
    outs = [
        "configure",
        ],
    cmd = """
cat > $@ <<"EOF"
#!/bin/sh
./external/secp256k1/autogen.sh >> /dev/null
EOF"""
)

filegroup(
    name = "all",
    srcs = glob(["**"]),
)

configure_make(
    name = "secp256k1",
    configure_options = [
        "--enable-module-recovery",
    ],
    lib_source = "@secp256k1//:all",
    static_libraries = [
        "libsecp256k1.a",
    ],
)

# cc_library(
#     name = "secp256k1",
#     srcs = [
#         # 'src/bench_internal.c', 'src/gen_context.c', 'src/bench_sign.c',
#         # 'src/bench_ecdh.c', 'src/bench_ecmult.c', 'src/bench_recover.c',
#         # 'src/bench_verify.c', 'src/secp256k1.c',
#         'src/bench_internal.c', 'src/gen_context.c', 'src/bench_sign.c', 'src/bench_ecdh.c', 'src/bench_ecmult.c', 'src/bench_recover.c', 'src/tests.c', 'src/tests_exhaustive.c', 'src/bench_verify.c', 'src/secp256k1.c',
#         'src/scalar_4x64.h', 'src/field_10x26.h', 'src/ecmult_gen_impl.h', 'src/libsecp256k1-config.h', 'src/hash_impl.h', 'src/num.h', 'src/field.h', 'src/ecmult_impl.h', 'src/bench.h', 'src/scalar_low.h', 'src/field_5x52_impl.h', 'src/scalar_low_impl.h', 'src/num_impl.h', 'src/field_5x52_int128_impl.h', 'src/ecdsa_impl.h', 'src/scalar_impl.h', 'src/testrand_impl.h', 'src/ecmult_const.h', 'src/group_impl.h', 'src/ecmult_const_impl.h', 'src/num_gmp_impl.h', 'src/field_10x26_impl.h', 'src/util.h', 'src/ecmult_gen.h', 'src/basic-config.h', 'src/group.h', 'src/field_5x52_asm_impl.h', 'src/hash.h', 'src/scalar_8x32.h', 'src/field_impl.h', 'src/ecmult.h', 'src/scratch_impl.h', 'src/eckey.h', 'src/scalar_8x32_impl.h', 'src/scalar_4x64_impl.h', 'src/num_gmp.h', 'src/scalar.h', 'src/testrand.h', 'src/scratch.h', 'src/field_5x52.h', 'src/eckey_impl.h', 'src/ecdsa.h', 'src/ecmult_static_context.h',
#     ],
#     local_defines = [
#         "USE_NUM_GMP=1",
#     ],
#     hdrs = glob([
#         "src/*.h",
#         "src/modules/recovery/*.h",
#         "include/*.h",
#     ]),
#     includes = [
#         "include",
#         "contrib",
#         "src",
#     ],
#     linkopts = [
#         "-lgmp",
#     ],
# )

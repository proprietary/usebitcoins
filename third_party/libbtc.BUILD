load("@rules_foreign_cc//tools/build_defs:configure.bzl", "configure_make")

filegroup(
    name = "all",
    srcs = glob(["**"]),
)

configure_make(
    name = "libbtc",
    configure_options = [
        "--disable-wallet",
        "--disable-tools",
    ],
    lib_source = "@com_github_libbtc_libbtc//:all",
    static_libraries = [
        "libbtc.a",
    ],
    visibility = ["//visibility:public"],
)

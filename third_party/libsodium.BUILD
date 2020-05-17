package(default_visibility = ["//visibility:public"])

load("@rules_foreign_cc//tools/build_defs:configure.bzl", "configure_make")

configure_make(
    name = "sodium",
    configure_options = [
        "--disable-shared",
        "--enable-static",
        "--disable-debug",
        "--disable-dependency-tracking",
        "--with-pic",
    ],
    lib_source = "@org_libsodium_libsodium//:.",
    static_libraries = ["libsodium.a"],
)

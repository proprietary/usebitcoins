cc_library(
	name = "uSockets",
	srcs = glob(["src/**/*.c", "src/**/*.h"]),
	hdrs = ["src/libusockets.h"],
	includes = [
		"src",
		"src/internal",
	],
	defines = [
		"LIBUS_USE_OPENSSL",
		"WITH_OPENSSL=1",
	],
	deps = [
		"@com_googlesource_boringssl_boringssl//:ssl",
		"@com_googlesource_boringssl_boringssl//:crypto",
	],
	linkopts = ["-pthread"],
	visibility = ["//visibility:public"],
)

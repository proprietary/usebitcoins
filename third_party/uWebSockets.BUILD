cc_library(
	name = "uWebSockets",
	srcs = glob(["src/*.h", "src/f2/*.hpp"]),
	hdrs = glob(["src/*.h"]),
	includes = ["src"],
	include_prefix = "uWebSockets",
	deps = [
		"@com_github_unetworking_usockets//:uSockets",
		"@net_zlib_zlib//:zlib",
	],
    visibility = ["//visibility:public"],
)

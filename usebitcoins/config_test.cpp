#include <config.hpp>
#include <cstdio>
#include <errno.h>
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <memory>
#include <stdexcept>
#include <stdlib.h>

namespace
{

// TODO: refactor this out into a utility class file
/**
 * RAII-style scoped temporary file that deletes itself
 *
 */
struct ScopedTempFile {
    std::filesystem::path path;
    ScopedTempFile()
    {
	char temp_basename[] = "temp_file_XXXXXX";
	if (!::mkstemp(&temp_basename[0])) {
	    if (errno == EEXIST) {
		throw std::runtime_error{
			"Could not create a unique temporary filename"};
	    } else if (errno == EINVAL) {
		throw std::runtime_error{"Last six characters of "
					 "`temp_basename` were not XXXXXX."};
	    } else {
		throw std::runtime_error{""};
	    }
	}
	path = std::filesystem::temp_directory_path() / std::filesystem::path{temp_basename};
    }
    ~ScopedTempFile()
    {
	if (std::filesystem::exists(path)) {
	    std::filesystem::remove(path);
	}
    }
};

TEST(TestConfig, LoadConfigFile)
{
    using usebitcoins::config::config_t;
    // write a sample config to a temporary file
    const char* config_test_contents = R"(
fullnode_rpcuser: user
fullnode_rpcpassword: DO_NOT_USE
fullnode_host: 127.0.0.1
)";
    ScopedTempFile temp_config_file{};
    {
	std::ofstream temp_file{temp_config_file.path,
				std::ios::out | std::ios::trunc};
	temp_file << config_test_contents;
    }
    auto config = config_t::load_config(temp_config_file.path);
    EXPECT_FALSE(config.fullnode_host.empty());
    EXPECT_NE(0, config.fullnode_port);
    EXPECT_EQ(config.fullnode_port, 8332);
    EXPECT_FALSE(config.fullnode_rpcuser.empty());
    EXPECT_FALSE(config.fullnode_rpcpassword.empty());
}

} // namespace

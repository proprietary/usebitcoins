#include <config.hpp>
#include <yaml-cpp/yaml.h>
#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

namespace usebitcoins
{
namespace config
{
config_t config_t::load_config(const std::ifstream& src) {
    config_t out {};
    try {
	std::ostringstream oss {};
	oss << src.rdbuf();
	const std::string& a = oss.str();
	YAML::Node root = YAML::Load(a);
	if (auto fullnode_host = root["fullnode_host"]; !fullnode_host) {
	    out.fullnode_host = "127.0.0.1";
	} else {
	    out.fullnode_host = fullnode_host.as<std::string>();
	}
	if (auto fullnode_port = root["fullnode_port"]; !fullnode_port) {
	    out.fullnode_port = 8332;
	} else {
	    out.fullnode_port = fullnode_port.as<int>();
	}
	out.fullnode_rpcuser = root["fullnode_rpcuser"].as<std::string>();
	out.fullnode_rpcpassword = root["fullnode_rpcpassword"].as<std::string>();
    } catch (YAML::InvalidNode& e) {
	std::cerr << "Configuration file has missing or impropertly formatted entries: "
		  << e.what()
		  << std::endl;
	std::terminate();
    } catch (YAML::BadFile& e) {
	std::cerr << "Config file provided does not exist or is inaccessible" << std::endl;
	std::terminate();
    }
    return out;
}

config_t config_t::load_config(std::filesystem::path path) {
    return config_t::load_config(std::ifstream {path, std::ios::in});
}
} // namespace config
} // namespace usebitcoins


#include <yaml-cpp/yaml.h>
#include <string>
#include <cstdlib>
#include <filesystem>

namespace usebitcoins
{
namespace config
{
class config_t {
public:
    std::string fullnode_host;
    int fullnode_port;
    std::string fullnode_rpcuser;
    // TODO: use secure allocator (perferably a polymorphic allocator)
    std::string fullnode_rpcpassword;
    // TODO: disable core dumps; on Linux this is...
    // #include <sys/resource.h>
    // setrlimit(RLIMIT_CORE, &(struct rlimit) {0, 0})
public:
    static config_t load_config(std::filesystem::path path);
    static config_t load_config(const std::ifstream& src);
};

} // namespace config
} // namespace usebitcoins


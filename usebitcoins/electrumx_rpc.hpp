#ifndef _ELECTRUMX_RPC_HPP
#define _ELECTRUMX_RPC_HPP
// JSON-RPC with ElectrumX servers (to validate Bitcoin transactions w/o a full
// node)
#include <rapidjson/document.h>
#include <string>
#include <vector>

namespace usebitcoins
{
namespace electrumx_rpc
{

struct electrumx_server_params_t {
    std::string hostname;
    std::string port;
};


auto check_mempool(const electrumx_server_params_t& server, const std::string& transaction_script) -> bool;

auto estimate_fee(const electrumx_server_params_t& server, int conf_target = 6) -> uint64_t;

auto confirmations(const electrumx_server_params_t& server, const std::string& transaction_script) -> int;

} // namespace electrumx_rpc
} // namespace usebitcoins


#endif // _ELECTRUMX_RPC_HPP



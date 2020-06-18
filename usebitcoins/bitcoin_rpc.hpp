#ifndef _BITCOIN_RPC_HPP
#define _BITCOIN_RPC_HPP

#include <memory>
#include <rapidjson/document.h>
#include <string>
#include <vector>

namespace usebitcoins
{
namespace bitcoin_rpc
{

struct fullnode_params_t {
    std::string user;
    std::string password;
    std::string host;
    int port;
};

// auto call(const fullnode_params_t& fullnode_params, const std::string& method,
// 	  const std::vector<std::string>& params)
// 	-> std::unique_ptr<rapidjson::Document>;

/// Find a transaction in the mempool; e.g., for 0-conf
auto check_mempool(const fullnode_params_t& fullnode_params, const std::string& txid) -> bool;

/// Count confirmations for transaction
auto confirmations(const fullnode_params_t& fullnode_params, const std::string& txid) -> int;

/// Estimates fee in sats per kilobyte needed within `conf_target` blocks
/// https://developer.bitcoin.org/reference/rpc/estimatesmartfee.html
auto estimate_fee(const fullnode_params_t& fullnode_params, int conf_target) -> uint64_t;

} // namespace bitcoin_rpc
} // namespace usebitcoins

#endif // _BITCOIN_RPC_HPP

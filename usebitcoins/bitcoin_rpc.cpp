#include "usebitcoins/bitcoin_rpc.hpp"
#include <algorithm>
#include <cassert>
#include <cfloat>
#include <cmath>
#include <curl/curl.h>
#include <iostream>
#include <memory>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/pointer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <sstream>
#include <string>
#include <vector>

namespace usebitcoins
{
namespace bitcoin_rpc
{

namespace
{
auto whole_number(double n) -> bool
{
    return ::fabs(n - ::round(n)) < LDBL_EPSILON;
}

auto create_json(const std::string& method,
		 std::vector<rapidjson::Value>& params) -> std::string
{
    rapidjson::Document input{};
    auto& allocator = input.GetAllocator();
    // create JSON RPC call
    input.SetObject();
    input.AddMember("jsonrpc", rapidjson::Value{}.SetString("1.0").Move(),
		    allocator);
    std::string id{"usebitcoins_call"};
    input.AddMember("id",
		    rapidjson::Value{rapidjson::kStringType}
			    .SetString(id.c_str(), id.size(), allocator)
			    .Move(),
		    allocator);
    input.AddMember("method",
		    rapidjson::Value{rapidjson::kStringType}
			    .SetString(method.c_str(), method.size(), allocator)
			    .Move(),
		    allocator);
    rapidjson::Value params_json_array{};
    params_json_array.SetArray();
    params_json_array.Reserve(params.size(), allocator);
    for (auto& param : params) {
	params_json_array.PushBack(param, allocator);
    }
    input.AddMember("params", params_json_array, allocator);
    // stringify JSON
    rapidjson::StringBuffer sb{};
    rapidjson::Writer<rapidjson::StringBuffer> writer{sb};
    input.Accept(writer);
    auto s = sb.GetString();
    return s;
}

// /// Make a low-level JSON-RPC request to Bitcoin full node
auto call(const fullnode_params_t& fullnode_params, const std::string& method,
	  std::vector<rapidjson::Value>& params)
	-> std::unique_ptr<rapidjson::Document>
{
    // create JSON-RPC request JSON
    auto request_json = create_json(method, params);
    // make network call
    std::unique_ptr<::CURL, decltype(&::curl_easy_cleanup)> curl{
	    ::curl_easy_init(), &::curl_easy_cleanup};
    std::stringstream ss;
    ss << "http://" << fullnode_params.host << ":" << fullnode_params.port;
    const std::string& fullnode_url = ss.str();
    ::curl_easy_setopt(curl.get(), CURLOPT_URL, fullnode_url.c_str());
    ::curl_easy_setopt(curl.get(), CURLOPT_POSTFIELDSIZE, request_json.size());
    ::curl_easy_setopt(curl.get(), CURLOPT_POSTFIELDS, request_json.data());
    std::stringstream userpwd_builder{};
    userpwd_builder << fullnode_params.user << ":" << fullnode_params.password;
    const std::string& userpwd = userpwd_builder.str();
    ::curl_easy_setopt(curl.get(), CURLOPT_USERPWD, userpwd.c_str());
    curl_write_callback cb = [](char* response, size_t size, size_t nmemb,
				void* userp) -> size_t {
	auto realsize = size * nmemb;
	auto& stream = *static_cast<std::vector<char>*>(userp);
	stream.insert(stream.end(), response, response + realsize);
	return realsize;
    };
    std::vector<char> buf{};
    ::curl_easy_setopt(curl.get(), CURLOPT_WRITEDATA, &buf);
    ::curl_easy_setopt(curl.get(), CURLOPT_WRITEFUNCTION, cb);
    ::curl_easy_perform(curl.get());
    buf.push_back('\0'); // insert null terminator
    auto out = std::make_unique<rapidjson::Document>(rapidjson::kObjectType);
    out->Parse(buf.data());
    return out;
}
} // namespace

auto estimate_fee(const fullnode_params_t& fullnode_params, int conf_target)
	-> uint64_t
{
    std::string method{"estimatesmartfee"};
    std::vector<rapidjson::Value> params{};
    rapidjson::Value conf_target_node{rapidjson::kNumberType};
    conf_target_node.SetInt(conf_target);
    params.emplace_back(std::move(conf_target_node));
    auto res = call(fullnode_params, method, params);
    rapidjson::Pointer jp{"/result/feerate"};
    // feerate is returned as BTC per kilobyte
    rapidjson::Value* feerate_node = jp.Get(*res);
    if (feerate_node == nullptr) {
	// TODO: Handle
	throw std::runtime_error{"JSON RPC error"};
    }
    double feerate_as_double = feerate_node->GetDouble();
    // convert BTC/kb to satoshis/byte
    static constexpr auto SATOSHIS_PER_BTC = 100'000'000;
    double sats_per_byte =
	    std::ceil(SATOSHIS_PER_BTC * feerate_as_double / 1024l);
    assert(sats_per_byte <= 0.0l || whole_number(sats_per_byte));
    return std::max(1UL, static_cast<uint64_t>(sats_per_byte));
}

} // namespace bitcoin_rpc
} // namespace usebitcoins

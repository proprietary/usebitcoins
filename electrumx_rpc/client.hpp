#ifndef _CLIENT_HPP
#define _CLIENT_HPP
#include <asio.hpp>
#include <string_view>
#include <string>
#include <vector>
#include <cstdio>
#include <algorithm>
#include <iostream>
#include <sstream>


namespace electrumx_rpc {

namespace net = ::asio;

void init() {
	net::io_context ioc{1};
	(void)ioc;
}

using electrumx_port_t = uint16_t;

struct electrumx_peer_t {
	std::string host;
	electrumx_port_t ssl_port;
	electrumx_port_t tcp_port;
};

class session_t {
public:
	explicit session_t(net::io_context& ioc) : ioc_{ioc} {}
	void reconnect();
	net::awaitable<void> discover_peers();
	net::awaitable<void> rpc_call(std::string method, std::string params);

private:
	net::io_context& ioc_;
	/// Default peers hardcoded from Electrum's hardcoded peer list
	/// https://raw.githubusercontent.com/spesmilo/electrum/master/electrum/servers.json
	static std::vector<electrumx_peer_t> const default_peers;
	std::vector<electrumx_peer_t> peers = default_peers;
};

// net::awaitable<void> make_request(net::io_context& ioc, net::ip::tcp::endpoint endpoint) {
// 	net::ip::tcp::socket stream{ioc};
// 	std::puts("ready...");
// 	co_await stream.async_connect(endpoint, net::use_awaitable);
// 	std::puts("connected");
// 	constexpr std::string_view hello = R"({"jsonrpc": "1.0", "id": "test", "method": "estimatesmartfee", "params": [6]})";
// 	std::cout << hello << std::endl;
// 	std::stringstream ss;
// 	ss << "POST / HTTP/1.1\r\n"
// 		"Host: localhost\r\n"
// 		"Content-Type: text/plain\r\n"
// 		"Content-Length: $$$\r\n"
// 		"Authorization: Basic dXNlcjpiYWRwYXNzd29yZA==\r\n"
// 		"\r\n"
// 	   << hello;
// 	auto req_text = ss.str();
// 	constexpr std::string_view sentinel {"$$$"};
// 	auto pos = req_text.find(sentinel);
// 	auto content_length = hello.size();
// 	auto content_length_as_str = std::to_string(content_length);
// 	req_text.replace(pos, sentinel.size(), content_length_as_str);
// 	std::puts(req_text.c_str());
// 	co_await net::async_write(stream, net::const_buffer{req_text.data(), req_text.size()}, net::use_awaitable);
// 	std::puts("back from write");
// 	std::array<char, 8096> buf;
// 	auto mut_buf = net::buffer(buf);
// 	co_await stream.async_read_some(mut_buf, net::use_awaitable);
// 	std::puts("back from read");
// 	std::fwrite(buf.data(), 1,buf.size(), stdout);
// 	std::fflush(stdout);
// 	stream.close();
// }

} // namespace electrumx_rpc

#endif // _CLIENT_HPP

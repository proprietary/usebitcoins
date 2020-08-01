#include "electrumx_rpc/client.hpp"
#include <chrono>

namespace net = ::asio;

namespace electrumx_rpc
{
std::vector<electrumx_peer_t> const session_t::default_peers = {
	electrumx_peer_t{
		.host = "blockstream.info",
		.ssl_port = 700,
		.tcp_port = 110,
	},
	electrumx_peer_t{
		.host = "electrum.blockstream.info",
		.ssl_port = 50002,
		.tcp_port = 50001,
	},
	electrumx_peer_t{
		.host = "electrum.aantonop.com",
		.ssl_port = 50002,
		.tcp_port = 50001,
	},
	electrumx_peer_t{
		.host = "electrum.bitkoins.nl",
		.ssl_port = 50512,
		.tcp_port = 50001,
	},
	electrumx_peer_t{
		.host = "fortress.qtornado.com",
		.ssl_port = 443,
		.tcp_port = 50001,
	},
	electrumx_peer_t{
		.host = "bitcoin.lukechilds.co",
		.ssl_port = 50002,
		.tcp_port = 50001,
	},
};

} // namespace electrumx_rpc

int main(int, char**) {
	try {
		net::io_context ioc{1};
		net::co_spawn(ioc.get_executor(), [&ioc]() -> net::awaitable<void> {
				co_await electrumx_rpc::make_request(ioc);
			}, net::detached);
		ioc.run();
	} catch (std::exception& e) {
		std::fprintf(stderr, "error: %s\n", e.what());
	}
	return 0;
}

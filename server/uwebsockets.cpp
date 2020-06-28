#include <uWebSockets/App.h>
#include <iostream>

int main(int, char**) {
	uWS::App().get("/", [](auto* res, auto* req) -> void {
		res->end("hellow world!");
	}).listen(8000, [](auto* token) {
		if (token) {
			std::cout << "listening on port " << 8000 << std::endl;
		}
	}).run();
	return EXIT_FAILURE;
}

#include <curl/curl.h>
#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <string_view>

namespace
{
struct curl_deleter {
    void operator()(CURL* p) const noexcept { ::curl_easy_cleanup(p); }
};

std::string GET_as_string(const std::string& url)
{
    std::string out{};
    std::unique_ptr<CURL, curl_deleter> curl_handle{::curl_easy_init()};
    if (curl_handle.get() == 0) {
	throw new std::runtime_error{"CURL handle failed to init"};
    }
    curl_easy_setopt(curl_handle.get(), CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl_handle.get(), CURLOPT_WRITEFUNCTION,
		     static_cast<curl_write_callback>(
			     [](char* buffer, size_t size, size_t nmemb,
				void* userp) -> size_t {
				 size_t realsize = size * nmemb;
				 std::string& s =
					 *static_cast<std::string*>(userp);
				 s.append(static_cast<char*>(buffer), realsize);
				 return realsize;
			     }));
    curl_easy_setopt(curl_handle.get(), CURLOPT_WRITEDATA, &out);
    auto res = curl_easy_perform(curl_handle.get());
    if (res != CURLE_OK) {
	throw new std::runtime_error{"CURL error"};
    }
    return out;
}

TEST(LibCURLTest, TestGETSmokeTest)
{
    // compare result from system curl executable and libcurl here
    // Note: requires curl to be installed
    auto ip = GET_as_string("http://checkip.amazonaws.com/");
    // perform the same command with system curl...
    std::string ip_from_system_curl{};
    {
	std::unique_ptr<FILE, decltype(&::pclose)> system_curl_result{
		::popen("curl -s http://checkip.amazonaws.com/", "r"),
		::pclose};
	ASSERT_FALSE(!system_curl_result);
	std::array<char, 64> buf{};
	while (fgets(buf.data(), buf.size(), system_curl_result.get()) !=
	       nullptr) {
	    ip_from_system_curl.append(buf.data());
	}
    }
    EXPECT_GT(ip.size(), 0);
    EXPECT_STREQ(ip.c_str(), ip_from_system_curl.c_str());
}
} // namespace

int main(int argc, char** argv)
{
    curl_global_init(CURL_GLOBAL_ALL);
    ::testing::InitGoogleTest(&argc, argv);
    auto res = RUN_ALL_TESTS();
    curl_global_cleanup();
    return res;
}

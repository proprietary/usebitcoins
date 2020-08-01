cmake_minimum_required(VERSION 3.16)

include(CMakeLists.local.txt)

add_subdirectory("${PROJECT_SOURCE_DIR}/third_party/googletest" "third_party/googletest")

add_subdirectory("${PROJECT_SOURCE_DIR}/third_party/yaml-cpp" "third_party/yaml-cpp")

find_package(Boost 1.69 REQUIRED COMPONENTS system thread)
find_package(sodium REQUIRED)
find_package(CURL REQUIRED)
find_package(RapidJSON REQUIRED)
find_package(OpenSSL REQUIRED)

add_library(usebitcoinslib)
target_include_directories(usebitcoinslib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_sources(usebitcoinslib PUBLIC
	price_oracle.hpp
	price_oracle.cpp
	genaddress.cpp
	genaddress.hpp
	hash.cpp
	hash.hpp
	bitcoin_rpc.hpp
	bitcoin_rpc.cpp
	electrumx_rpc.hpp
	electrumx_rpc.cpp
)
# TODO add rapidjson here
target_link_libraries(usebitcoinslib PUBLIC Boost::headers sodium curl ${OPENSSL_LIBRARIES})
target_compile_features(usebitcoinslib PUBLIC cxx_std_20)

add_library(usebitcoins_server)
target_sources(usebitcoins_server PUBLIC
  config.hpp
  config.cpp
)
target_link_libraries(usebitcoins_server PUBLIC usebitcoinslib yaml-cpp::yaml-cpp)
target_include_directories(usebitcoins_server PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_features(usebitcoins_server PUBLIC cxx_std_20)

include(CTest)
enable_testing()
include(GoogleTest)

add_executable(usebitcoins_price_oracle_test)
target_sources(usebitcoins_price_oracle_test PRIVATE price_oracle.hpp price_oracle.cpp price_oracle_test.cpp)
target_link_libraries(usebitcoins_price_oracle_test PRIVATE usebitcoinslib gtest gtest_main)
#gtest_add_tests(TARGET usebitcoins_price_oracle_test)

add_executable(usebitcoins_hash_test)
target_sources(usebitcoins_hash_test PRIVATE hash_test.cpp)
target_link_libraries(usebitcoins_hash_test PRIVATE usebitcoinslib gtest gmock gtest_main)
target_compile_features(usebitcoins_hash_test PUBLIC cxx_std_20)

add_executable(usebitcoins_libcurl_test)
target_sources(usebitcoins_libcurl_test PRIVATE libcurl_test.cpp)
target_link_libraries(usebitcoins_libcurl_test PRIVATE curl gtest gtest_main)
target_compile_features(usebitcoins_libcurl_test PUBLIC cxx_std_20)

add_executable(usebitcoins_bitcoin_rpc_test)
target_sources(usebitcoins_bitcoin_rpc_test PRIVATE bitcoin_rpc.hpp bitcoin_rpc_test.cpp)
target_link_libraries(usebitcoins_bitcoin_rpc_test PRIVATE curl gtest gtest_main usebitcoinslib)
target_compile_features(usebitcoins_bitcoin_rpc_test PUBLIC cxx_std_20)

add_executable(usebitcoins_electrumx_rpc_test)
target_sources(usebitcoins_electrumx_rpc_test PRIVATE electrumx_rpc.hpp electrumx_rpc.test.cpp)
target_link_libraries(usebitcoins_electrumx_rpc_test PRIVATE gtest gtest_main usebitcoinslib)
target_compile_features(usebitcoins_electrumx_rpc_test PUBLIC cxx_std_20)

add_executable(usebitcoins_config_test)
target_link_libraries(usebitcoins_config_test PRIVATE usebitcoins_server gtest gtest_main)
target_include_directories(usebitcoins_config_test PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
target_sources(usebitcoins_config_test PRIVATE config_test.cpp config.hpp)
target_compile_features(usebitcoins_config_test PRIVATE cxx_std_20)


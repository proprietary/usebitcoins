cmake_minimum_required(VERSION 3.16)

add_subdirectory("${PROJECT_SOURCE_DIR}/third_party/googletest" "third_party/googletest")

find_package(Boost 1.69 REQUIRED COMPONENTS system thread)
find_package(sodium REQUIRED)
find_package(CURL REQUIRED)

add_library(usebitcoinslib)
target_include_directories(usebitcoinslib PUBLIC ".")
target_sources(usebitcoinslib PUBLIC
	price_oracle.hpp
	price_oracle.cpp
	genaddress.cpp
	genaddress.hpp
	hash.cpp
	hash.hpp
)
target_link_libraries(usebitcoinslib PUBLIC Boost::headers sodium CURL::libcurl)
target_compile_features(usebitcoinslib PUBLIC cxx_std_17)

add_executable(usebitcoins_hash_test)
target_sources(usebitcoins_hash_test PRIVATE hash_test.cpp)
target_link_libraries(usebitcoins_hash_test PRIVATE usebitcoinslib gtest gmock gtest_main)
target_compile_features(usebitcoins_hash_test PUBLIC cxx_std_17)

add_executable(usebitcoins_beast_test)
target_sources(usebitcoins_beast_test PRIVATE beast.cpp)
target_link_libraries(usebitcoins_beast_test PRIVATE Boost::headers Boost::system Boost::thread)
target_compile_features(usebitcoins_beast_test PUBLIC cxx_std_17)

add_executable(usebitcoins_beast_test2)
target_sources(usebitcoins_beast_test2 PRIVATE beast2.cpp)
target_link_libraries(usebitcoins_beast_test2 PRIVATE Boost::headers Boost::system Boost::thread)
target_compile_features(usebitcoins_beast_test2 PUBLIC cxx_std_17)

add_executable(usebitcoins_libcurl_test)
target_sources(usebitcoins_libcurl_test PRIVATE libcurl_test.cpp)
target_link_libraries(usebitcoins_libcurl_test PRIVATE curl gtest gtest_main)
target_compile_features(usebitcoins_libcurl_test PUBLIC cxx_std_20)

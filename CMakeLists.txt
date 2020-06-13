cmake_minimum_required(VERSION 3.15)

project(usebitcoins LANGUAGES CXX)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})


add_subdirectory(usebitcoins)

# include(FetchContent)
# FetchContent_Declare(rapidjson
#   URL https://github.com/Tencent/rapidjson/archive/v1.1.0.tar.gz
#   URL_HASH SHA256=bf7ced29704a1e696fbccf2a2b4ea068e7774fa37f6d7dd4039d0787f8bed98
# )
# FetchContent_GetProperties(rapidjson)
# if (NOT rapidjson_POPULATED)
#   FetchContent_Populate(rapidjson)
#   add_library(rapidjson INTERFACE)
#   add_library(rapidjson::rapidjson ALIAS rapidjson)
#   target_include_directories(rapidjson SYSTEM INTERFACE ${rapidjson_SOURCE_DIR}/include)
#   target_compile_definitions(rapidjson INTERFACE RAPIDJSON_HAS_STDSTRING=1)
# endif()


# add_executable(bip32_test)
# target_sources(bip32_test PUBLIC "usebitcoins/bip32_test.cpp")


cmake_minimum_required(VERSION 3.15)

project(usebitcoins LANGUAGES CXX)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})


add_subdirectory(usebitcoins)


# add_executable(bip32_test)
# target_sources(bip32_test PUBLIC "usebitcoins/bip32_test.cpp")


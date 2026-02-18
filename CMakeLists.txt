cmake_minimum_required(VERSION 3.20)
project(concurrent_kv LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(concurrent_kv src/main.cpp)
target_include_directories(concurrent_kv PRIVATE ${CMAKE_SOURCE_DIR}/include)


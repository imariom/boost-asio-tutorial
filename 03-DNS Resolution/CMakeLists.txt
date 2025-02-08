cmake_minimum_required(VERSION 3.15)
project(boost-asio-tutorial)

find_package(Boost REQUIRED)
if(NOT Boost_FOUND)
    message(FATAL "C++ Boost libraries installed")
endif()

# Synchronous DNS resolution target
add_executable(sync sync_dns.cc)
target_include_directories(sync PRIVATE Boost::asio)

# Asynchronous DNS resolution target
add_executable(async async_dns.cc)
target_include_directories(async PRIVATE Boost::asio)
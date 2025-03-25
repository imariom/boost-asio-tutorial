cmake_minimum_required(VERSION 3.15)
project(boost-asio-tutorial)

find_package(Boost REQUIRED)
if(NOT Boost_FOUND)
    message(FATAL "C++ Boost libraries not installed")
endif()

add_executable(server main.cc)
target_include_directories(server PRIVATE Boost::asio)
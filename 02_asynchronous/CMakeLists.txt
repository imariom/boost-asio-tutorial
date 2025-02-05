cmake_minimum_required(VERSION 3.15)
project(boost-asio-tutorial)

find_package(Boost REQUIRED)
if(NOT Boost_FOUND)
    message(FATAL "C++ Boost libraries installed")
endif()

add_executable(app main.cc)
target_include_directories(app PRIVATE Boost::asio)
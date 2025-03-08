cmake_minimum_required(VERSION 3.15)
project(boost-asio-tutorial)

find_package(Boost REQUIRED)
if(NOT Boost_FOUND)
    message(FATAL "C++ Boost libraries not installed")
endif()

# Build the client code
add_executable(client client.cc)
target_include_directories(client PRIVATE Boost::asio)

# Build the server code
add_executable(server server.cc)
target_include_directories(server PRIVATE Boost::asio)
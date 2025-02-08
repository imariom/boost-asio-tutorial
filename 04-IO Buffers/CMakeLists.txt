cmake_minimum_required(VERSION 3.15)
project(boost-asio-tutorial)

find_package(Boost REQUIRED)
if(NOT Boost_FOUND)
    message(FATAL "C++ Boost libraries installed")
endif()

# Build RAW array example
add_executable(raw raw.cc)
target_include_directories(raw PRIVATE Boost::asio)

# Build std::vector example
add_executable(vector vector.cc)
target_include_directories(vector PRIVATE Boost::asio)

# Build std::array example
add_executable(array array.cc)
target_include_directories(array PRIVATE Boost::asio)

# Build server code
add_executable(server server.cc)
target_include_directories(server PRIVATE Boost::asio)

# Build client code
add_executable(client client.cc)
target_include_directories(client PRIVATE Boost::asio)

# Build scatter-gather example
add_executable(scatter scatter.cc)
target_include_directories(scatter PRIVATE Boost::asio)
cmake_minimum_required(VERSION 3.15)
project(async-tcp-client)

find_package(Boost REQUIRED)
if(NOT Boost_FOUND)
    message(FATAL "Boost libraries not found")
endif()

add_executable(app main.cc)
target_include_directories(app PRIVATE Boost::asio)
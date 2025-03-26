cmake_minimum_required(VERSION 3.15)
project(chatito)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED)
if(NOT Boost_FOUND)
    message(FATAL "C++ Boost Libraries not found")
endif()

add_subdirectory(app)
cmake_minimum_required(VERSION 3.15)
project(restpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(RESTPP_VERSION_MAJOR 1)
set(RESTPP_VERSION_MINOR 0)
set(RESTPP_VERSION_REVISION 0)

set(RESTPP_EXPORT_DIR cmake/restpp CACHE STRING "Directory to install CMake config files.")
set(RESTPP_INSTALL_HEADERS OFF CACHE BOOL "Install header files.")
set(RESTPP_INSTALL ON CACHE BOOL "Add install commands.")
set(BUILD_EXAMPLES ON)
set(BUILD_TESTS ON)

find_package(Boost REQUIRED)
if(NOT Boost_FOUND)
    message(FATAL "C++ Boost libraries not installed")
endif()

add_library(restpp INTERFACE)
target_include_directories(
    restpp
    INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
              $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

if(RESTPP_INSTALL)
    install(TARGETS restpp
            EXPORT ${PROJECT_NAME}_Targets
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(BUILD_EXAMPLES)
    add_subdirectory(example)
endif()
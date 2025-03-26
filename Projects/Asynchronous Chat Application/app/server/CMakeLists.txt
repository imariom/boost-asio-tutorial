project(chatito-server)

add_executable(chatitos

    main.cpp
    ${CMAKE_SOURCE_DIR}/src/Server.cpp)

target_include_directories(chatitos
    PRIVATE ${CMAKE_SOURCE_DIR}/include/
    PRIVATE Boost::asio)
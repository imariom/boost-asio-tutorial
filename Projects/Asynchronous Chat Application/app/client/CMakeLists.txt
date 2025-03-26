project(chatito-client)

add_executable(chatitoc

    main.cpp
    ${CMAKE_SOURCE_DIR}/src/Client.cpp)

target_include_directories(chatitoc
    PRIVATE ${CMAKE_SOURCE_DIR}/include/
    PRIVATE Boost::asio)
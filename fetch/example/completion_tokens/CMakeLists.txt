add_executable(completion main.cc)
target_include_directories(completion PRIVATE Boost::asio)
target_link_libraries(completion 
    PRIVATE restpp)

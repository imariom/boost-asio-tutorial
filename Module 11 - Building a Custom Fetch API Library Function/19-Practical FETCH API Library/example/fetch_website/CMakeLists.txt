add_executable(fetch_website main.cc)
target_include_directories(fetch_website PRIVATE Boost::asio)
target_link_libraries(fetch_website 
    PRIVATE restpp)


#include <iostream>
#include <thread>
#include <chrono>
#include <boost/asio.hpp>

#include "bro.hpp"

using namespace std::chrono_literals;

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "Usage: app <url>" << std::endl;
        return -1;
    }
    std::string url = argv[1];

    boost::asio::io_context ioCtx;

    std::vector<std::thread> pool;
    for (auto i = 0; i != 4; ++i)
        pool.push_back(
            std::thread([&] { ioCtx.run(); })
        );

    // bro::fetch(ioCtx, "www.google.com",
    //     [](const boost::system::error_code& ec, bro::http_response response)
    //     {
    //         if (!ec) {
    //             std::cout << response.headers << std::endl;
    //         } else {
    //             std::cerr << "Error: " << ec.message() << std::endl;
    //             std::cerr << ec.what() << std::endl;
    //         }
    //     });

    // for (auto i = 0; i != 5; ++i) {
    //     std::this_thread::sleep_for(0.5s);
    //     std::cout << "main() ..." << std::endl;
    // }

    auto future = bro::fetch(ioCtx, url, boost::asio::use_future);

    for (auto i = 0; i != 5; ++i) {
        std::this_thread::sleep_for(0.5s);
        std::cout << "main() ..." << std::endl;
    }

    try
    {
        auto response = future.get();
        std::cout << response.headers << std::endl;
        // ...
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    std::getchar();
        
    return 0;
}
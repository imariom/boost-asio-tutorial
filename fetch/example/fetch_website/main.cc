#include <boost/asio.hpp>
#include <restpp/fetch.hpp>

#include <iostream>
#include <thread>
#include <chrono>

int main(int argc, char* argv[])
{
    if (argc < 3) {
        std::cerr << "Usage: fetch <host> <path>" << std::endl;
        return 0;
    }
    std::string host = argv[1];
    std::string path = argv[2];

    boost::asio::io_context ioCtx;
    restpp::fetch(ioCtx, host, path,
        [](const boost::system::error_code& ec, restpp::http_response response)
        {
            if (!ec) {
                std::cout << response.body << std::endl;
            } else {
                std::cerr << "Error: " << ec.message() << std::endl;
                std::cerr << ec.what() << std::endl;
            }
        });

    // Simulate long running operation in the main function
    for (auto i = 0; i != 3; ++i) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "main() ..." << std::endl;
    }

    ioCtx.run();
    return 0;
}

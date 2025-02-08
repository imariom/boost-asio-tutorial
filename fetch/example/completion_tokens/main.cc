#include <iostream>
#include <chrono>
#include <thread>
#include <boost/asio.hpp>

#include <restpp/fetch.hpp>

void test_callback(std::string url, std::string path)
{
    boost::asio::io_context ioCtx;
    restpp::fetch(ioCtx, url, path,
        [](const boost::system::error_code& ec, restpp::http_response response)
        {
            if (!ec) {
                std::cout << "Callback: " << std::endl;
                std::cout << response.headers << std::endl;
                std::cout << response.body << std::endl;
            } else {
                std::cerr << "Callback: " << std::endl;
                std::cerr << "Error: " << ec.message() << std::endl;
                std::cerr << ec.what() << std::endl;
            }
        });

    ioCtx.run();
}

void test_future(std::string url, std::string path)
{
    boost::asio::io_context ioCtx;
    std::future<restpp::http_response> f =
        restpp::fetch(ioCtx, url, path, boost::asio::use_future);
    ioCtx.run();

    try
    {
        restpp::http_response response = f.get();
        std::cout << "Future: " << std::endl;
        std::cout << response.headers << std::endl;
        std::cout << response.body << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Future: " << std::endl;
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}

void test_deferred(std::string url, std::string path)
{
    boost::asio::io_context ioCtx;
    auto operation = restpp::fetch(ioCtx, url, path, boost::asio::deferred);

    std::move(operation)(
        [](const boost::system::error_code& ec, restpp::http_response response)
        {
            std::cout << "Deffered: " << std::endl;
            if (!ec) {
                std::cout << response.headers << std::endl;
                std::cout << response.body << std::endl;
            } else {
                std::cerr << "Error: " << ec.message() << std::endl;
                std::cerr << ec.what() << std::endl;
            }
        }
    );

    ioCtx.run();
}

int main(int argc, char* argv[])
{
    if (argc < 3) {
        std::cerr << "Usage: app <url> <path>" << std::endl;
        return -1;
    }
    std::string url = argv[1];
    std::string path = argv[2];

    test_callback(url, path);
    test_future(url, path);
    test_deferred(url, path);
        
    return 0;
}

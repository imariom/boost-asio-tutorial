#include <boost/asio.hpp>
#include <iostream>

using boost::asio::ip::tcp;

int main() {
    try {
        // Create a TCP endpoint for the server (listening on port 12345)
        // in IP addresses - FOR SERVERS
        tcp::endpoint server_endpoint(tcp::v4(), 54321);

        std::cout << "Server endpoint: " << server_endpoint << std::endl;
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
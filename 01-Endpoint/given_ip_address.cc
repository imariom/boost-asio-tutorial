#include <boost/asio.hpp>
#include <iostream>

using boost::asio::ip::tcp;

int main() {
    try {
        // Create an IPv4 address (e.g., 127.0.0.1 for localhost)
        boost::asio::ip::address ip_address = boost::asio::ip::make_address("127.0.0.1");

        // Create a TCP endpoint for the server (listening on port 12345)
        tcp::endpoint server_endpoint(ip_address, 54321);

        // Create a TCP endpoint for the client (connecting to the server's IP and port)
        tcp::endpoint client_endpoint(ip_address, 54321);

        std::cout << "Server endpoint: " << server_endpoint << std::endl;
        std::cout << "Client endpoint: " << client_endpoint << std::endl;
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
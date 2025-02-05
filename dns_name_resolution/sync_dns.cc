#include <boost/asio.hpp>
#include <iostream>

namespace asio = boost::asio;
using tcp = asio::ip::tcp;
using udp = asio::ip::udp;

int main() {
    try {
        asio::io_context io_context;

        // Create a resolver
        tcp::resolver resolver(io_context);

        // Resolve the DNS name (e.g., "www.example.com") and service (e.g., "http")
        tcp::resolver::results_type endpoints = resolver.resolve("www.google.com", "http");

        udp::resolver resl{io_context};
        udp::resolver::results_type ends = resl.resolve("example.com", "http");

        // Iterate over the resolved endpoints
        for (const auto& endpoint : endpoints) {
            std::cout << "Resolved endpoint: " << endpoint.endpoint() << std::endl;
        }

        // Use the first endpoint to create a socket and connect
        tcp::socket socket(io_context);
        asio::connect(socket, endpoints);

        std::cout << "Connected to: " << socket.remote_endpoint() << std::endl;
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
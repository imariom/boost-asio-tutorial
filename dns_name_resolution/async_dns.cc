#include <boost/asio.hpp>
#include <iostream>

namespace asio = boost::asio;
using tcp = asio::ip::tcp;

int main() {
    asio::io_context io_context;

    // Create a resolver
    tcp::resolver resolver(io_context);

    // Asynchronously resolve the DNS name
    resolver.async_resolve("www.google.com", "http",
        [&](const boost::system::error_code& ec, tcp::resolver::results_type endpoints) {
            if (ec) {
                std::cerr << "Error resolving DNS: " << ec.message() << std::endl;
                return;
            }

            // Iterate over the resolved endpoints
            for (const auto& endpoint : endpoints) {
                std::cout << "Resolved endpoint: " << endpoint.endpoint() << std::endl;
            }

            // Use the first endpoint to create a socket and connect
            auto socket = std::make_shared<tcp::socket>(io_context);
            asio::async_connect(*socket, endpoints,
                [socket](const boost::system::error_code& ec, const tcp::endpoint&) {
                    if (ec) {
                        std::cerr << "Error connecting: " << ec.message() << std::endl;
                        return;
                    }

                    std::cout << "Connected to: " << socket->remote_endpoint() << std::endl;
                });
        });

    // Run the io_context to start the asynchronous operation
    io_context.run();

    return 0;
}
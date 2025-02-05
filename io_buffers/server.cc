#include <boost/asio.hpp>
#include <iostream>

using boost::asio::ip::tcp;

int main() {
    try {
        boost::asio::io_context io_context;

        // Create a server endpoint (listening on port 12345)
        tcp::endpoint endpoint(tcp::v4(), 12345);
        tcp::acceptor acceptor(io_context, endpoint);

        std::cout << "Server is listening on " << endpoint << std::endl;

        // Wait for a client to connect
        tcp::socket socket(io_context);
        acceptor.accept(socket);

        std::cout << "Client connected!" << std::endl;

        // Receive data from the client
        char receive_data[1024];
        size_t length = socket.read_some(boost::asio::buffer(receive_data));
        std::cout << "Received: " << std::string(receive_data, length) << std::endl;

        // Send data to the client
        std::string send_data = "Hello from server!";
        boost::asio::write(socket, boost::asio::buffer(send_data));
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
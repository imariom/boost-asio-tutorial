#include <boost/asio.hpp>
#include <iostream>

using boost::asio::ip::tcp;

int main() {
    try {
        boost::asio::io_context io_context;

        // Create a client endpoint (connecting to the server's IP and port)
        tcp::endpoint endpoint(boost::asio::ip::make_address("127.0.0.1"), 12345);

        // Create a socket for the client
        tcp::socket socket(io_context);

        // Connect to the server (blocking-operation)
        socket.connect(endpoint);

        std::cout << "Connected to server at " << endpoint << std::endl;

        // Receive a message from the server
        char data[1024];
        size_t length = socket.read_some(boost::asio::buffer(data));
        std::cout << "Received from server: " << std::string(data, length) << std::endl;

        // Send a message to the server
        std::string message = "Hello from client!";
        boost::asio::write(socket, boost::asio::buffer(message));
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
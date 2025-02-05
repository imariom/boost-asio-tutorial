#include <boost/asio.hpp>
#include <iostream>

using boost::asio::ip::tcp;

int main() {
    try {
        boost::asio::io_context io_context;

        // Create a server endpoint (listening on port 12345)
        tcp::endpoint endpoint(tcp::v4(), 12345);

        // Create an acceptor to listen for incoming connections
        tcp::acceptor acceptor(io_context, endpoint);

        std::cout << "Server is listening on " << endpoint << std::endl;

        // Create a socket for the server
        tcp::socket socket(io_context);

        // Wait for a client to connect (blocking operation)
        acceptor.accept(socket);

        std::cout << "Client connected from: " << socket.remote_endpoint() << std::endl;

        // Send a message to the client
        std::string message = "Hello from server!";
        boost::asio::write(socket, boost::asio::buffer(message));

        // Receive a message from the client
        char data[1024];
        size_t length = socket.read_some(boost::asio::buffer(data));
        std::cout << "Received from client: " << std::string(data, length) << std::endl;
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
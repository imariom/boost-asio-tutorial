#include <boost/asio.hpp>
#include <iostream>

using boost::asio::ip::tcp;

int main() {
    try {
        boost::asio::io_context io_context;

        // Create a client endpoint (connecting to the server's IP and port)
        tcp::endpoint endpoint(boost::asio::ip::make_address("127.0.0.1"), 12345);

        // Create a socket and connect to the server
        tcp::socket socket(io_context);
        socket.connect(endpoint);

        std::cout << "Connected to server!" << std::endl;

        // Send data to the server
        std::string send_data = "Hello from client!";
        boost::asio::write(socket, boost::asio::buffer(send_data));

        // Receive data from the server
        char receive_data[1024];
        size_t length = socket.read_some(boost::asio::buffer(receive_data));
        std::cout << "Received: " << std::string(receive_data, length) << std::endl;
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
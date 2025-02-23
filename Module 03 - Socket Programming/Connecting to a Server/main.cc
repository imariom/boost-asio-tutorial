#include <iostream>

#include <boost/asio.hpp>

int main(int argc, char* argv[])
{
    // Get server IP address and port number from the command line arguments
    if (argc < 3)
    {
        std::cerr << "Usage: client <server-ip-address> <port-number>\n";
        return -1;
    }
    std::string serverIPAddress = argv[1];
    std::size_t portNumber = std::atoi(argv[2]);

    // Create the I/O execution context
    boost::asio::io_context io_context;
    
    try
    {
        // Create an endpoint that represent the remote server
        boost::asio::ip::tcp::endpoint endpoint(
            boost::asio::ip::make_address(serverIPAddress), portNumber);

        // Create a socket for the client
        boost::asio::ip::tcp::socket socket(io_context);

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
    }
    catch (std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
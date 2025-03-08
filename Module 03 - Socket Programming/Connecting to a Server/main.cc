#include <iostream>

#include <boost/asio.hpp>

int main(int argc, char* argv[])
{
    // Check command line arguments.
    if (argc != 3)
    {
        std::cerr <<
            "Usage: client <server-ip-address> <port-number>\n" <<
            "Example:\n" <<
            "   client 127.0.0.1 54321\n";
        return EXIT_FAILURE;
    }
    
    // Create the I/O execution context
    boost::asio::io_context ioCtx;
    
    try
    {
        // Create an endpoint that represent the remote server
        boost::asio::ip::tcp::endpoint serverEndpoint(
            boost::asio::ip::make_address(argv[1]), std::atoi(argv[2]));

        // Create an active socket
        boost::asio::ip::tcp::socket socket(ioCtx);

        // Connect to the server (blocking-operation)
        socket.connect(serverEndpoint);
        std::cout << "[CONNECTED TO] " << serverEndpoint << '\n';

        // Receive a message from ihe server
        char data[1024];
        size_t length = socket.read_some(boost::asio::buffer(data));
        std::cout << "[RECEIVED] " << std::string(data, length) << '\n';

        // Send a message to the server
        std::string message = "Hello from client!";
        boost::asio::write(socket, boost::asio::buffer(message));
        std::cout << "[SENT] " << message << '\n';
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
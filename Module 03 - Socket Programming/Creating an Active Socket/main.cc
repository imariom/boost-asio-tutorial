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
    boost::asio::io_context ioCtx;

    try
    {
        // Create a client endpoint provided the IP and port number
        boost::asio::ip::tcp::endpoint endpoint(
            boost::asio::ip::make_address(serverIPAddress), portNumber);

        // Create an active socket (client)
        boost::asio::ip::tcp::socket socket(ioCtx);

        // Connect to the server (blocking-operation)
        socket.connect(endpoint);
        std::cout << "Connected to server at " << endpoint << std::endl;

        // ... socket can now be used for I/O operations
    }
    catch (std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
#include <iostream>

#include <boost/asio.hpp>

int main(int argc, char** argv)
{
    // Check command line arguments.
    if (argc != 3)
    {
        std::cerr <<
            "Usage: client <server-ip-address> <port-number>\n" <<
            "Example:\n" <<
            "   client 182.242.224.3 43280\n";
        return EXIT_FAILURE;
    }

    // Create the I/O execution context.
    boost::asio::io_context ioCtx;

    // Create an active socket (client)
    boost::asio::ip::tcp::socket socket(ioCtx);

    try
    {
        // Create the server IP address.
        boost::asio::ip::address serverIPAddress =
            boost::asio::ip::make_address(argv[1]);

        // Get the server port number.
        std::size_t portNumber = std::atoi(argv[2]);
        
        // Create the server TCP endpoint.
        boost::asio::ip::tcp::endpoint serverEndpoint(
            serverIPAddress, portNumber);

        // Connect to the server (blocking-operation).
        socket.connect(serverEndpoint);

        std::cout << "Connected to server: " << serverEndpoint << std::endl;

        // ... socket can now be used for I/O operations
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
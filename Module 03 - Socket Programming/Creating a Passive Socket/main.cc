#include <iostream>

#include <boost/asio.hpp>

int main(int argc, char** argv)
{
    // Check command line arguments.
    if (argc != 2)
    {
        std::cerr <<
            "Usage: server <port-number>\n" <<
            "Example:\n" <<
            "   server 54321\n";
        return EXIT_FAILURE;
    }

    // Create the I/O execution context.
    boost::asio::io_context ioCtx;
    
    try
    {
        // Create a server endpoint in all of the host IPv4 IP addresses
        // and a provided portNumber.
        boost::asio::ip::tcp::endpoint endpoint(
            boost::asio::ip::tcp::v4(), std::atoi(argv[1]));

        // Create an acceptor to listen for incoming connections.
        boost::asio::ip::tcp::acceptor acceptor(ioCtx, endpoint);

        std::cout << "Server is listening on " << endpoint << std::endl;

        // Create a socket for the connected client
        boost::asio::ip::tcp::socket socket(ioCtx);

        // Wait for a client to connect (blocking operation)
        acceptor.accept(socket);

        std::cout << "Client connected from: "
            << socket.remote_endpoint() << '\n';

        // ... socket can now be used for I/O operations
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
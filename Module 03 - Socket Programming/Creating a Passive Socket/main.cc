#include <iostream>

#include <boost/asio.hpp>

int main(int argc, char* argv[])
{
    // Get the server port number from the command line arguments
    if (argc < 2)
    {
        std::cerr << "Usage: server <port-number>\n";
        return -1;
    }
    std::size_t portNumber = std::atoi(argv[2]);

    // Create the I/O execution context
    boost::asio::io_context ioCtx;
    
    try
    {
        // Create a server endpoint in all of the host IPv4 IP addresses
        // and a provided portNumber.
        boost::asio::ip::tcp::endpoint endpoint(
            boost::asio::ip::tcp::v4(), portNumber);

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
    catch (std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
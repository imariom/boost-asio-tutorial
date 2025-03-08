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
    
    // Create the I/O execution context
    boost::asio::io_context ioCtx;

    // Create the endpoint
    boost::asio::ip::tcp::endpoint endpoint(
        boost::asio::ip::tcp::v4(), std::atoi(argv[1]));

    // Create a passive socket
    boost::system::error_code ec;

    boost::asio::ip::tcp::acceptor acceptor(ioCtx);
    
    // Open the socket
    acceptor.open(endpoint.protocol(), ec);

    if (ec) {
        // Failed to open the acceptor socket.
        std::cerr
            << "Error: " << ec.what()
            << ". Message: " << ec.message() << "\n";

        return ec.value();
    }

    // Bind the acceptor to an endpoint
    acceptor.bind(endpoint, ec);

    if (ec) {
        // Failed to bind the acceptor socket.
        std::cerr
            << "Error: " << ec.what()
            << ". Message: " << ec.message() << "\n";

        return ec.value();
    }

    std::cout << "Socket bound to: " << endpoint << '\n';

    // ... then listen, accept and do I/O
    
    return EXIT_SUCCESS;
}
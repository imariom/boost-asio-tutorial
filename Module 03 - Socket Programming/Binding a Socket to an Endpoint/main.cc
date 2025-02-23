#include <iostream>
#include <string>

#include <boost/asio.hpp>

int main(int argc, char* argv[])
{
    // Get server port number from the command line arguments
    if (argc < 2)
    {
        std::cerr << "Usage: server <port-number>\n";
        return -1;
    }
    std::size_t portNumber = std::atoi(argv[1]);
    
    // Create the I/O execution context
    boost::asio::io_context ioCtx;

    // Create a passive socket
    boost::asio::ip::tcp::acceptor acceptor(ioCtx);

    // Bind the acceptor to an endpoint
    boost::system::error_code ec;
    acceptor.bind(boost::asio::ip::tcp::endpoint(
        boost::asio::ip::tcp::v4(), portNumber), ec);

    if (ec) {
        // Failed to bind the acceptor socket.
        std::cerr << "Error: " << ec.what() << ". Message"
            << ec.message() << "\n";

        return ec.value();
    }

    // ... then listen, accept and do I/O
    
    return 0;
}
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
    
    // Create a TCP endpoint for the server for all host IPv4 addresses
    // in a provided portNumber.
    boost::asio::ip::tcp::endpoint endpoint(
        boost::asio::ip::tcp::v4(), portNumber);

    std::cout << "Server endpoint: " << endpoint << std::endl;

    // ... the endpoint can be used by the server to listen
    // for incomming connections.

    return 0;
}
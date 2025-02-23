#include <iostream>
#include <string>

#include <boost/asio.hpp>

int main(int argc, char* argv[])
{
    // Get server port number from the command line arguments
    if (argc < 3)
    {
        std::cerr << "Usage: server <server-ip-address> <port-number>\n";
        return -1;
    }

    std::size_t portNumber = std::atoi(argv[1]);

    // Create an IP to represent the server address
    boost::asio::ip::address ipAddress =
        boost::asio::ip::make_address("127.0.0.1");
    
    // Create a TCP endpoint for the server for all host IPv4 addresses
    // in a provided portNumber.
    boost::asio::ip::tcp::endpoint endpoint(
        ipAddress, portNumber);

    std::cout << "Server endpoint: " << endpoint << std::endl;

    // ... the endpoint can be used by the server to listen
    // for incomming connections.

    return 0;
}
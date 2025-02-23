#include <iostream>
#include <boost/asio.hpp>

int main(int argc, char* argv[])
{
    // Get server port number from the command line arguments
    if (argc < 3)
    {
        std::cerr << "Usage: client <server-ip-address> <port-number>\n";
        return -1;
    }
    std::string serverIPAddress = argv[1];
    std::size_t portNumber = std::atoi(argv[2]);
    
    // Create an IPv4 address
    boost::asio::ip::address ipAddress =
        boost::asio::ip::make_address(serverIPAddress);

    // Create a TCP endpoint for the server (listening on port 54321)
    boost::asio::ip::tcp::endpoint serverEndpoint(ipAddress, portNumber);
    std::cout << "Server endpoint: " << serverEndpoint << "\n";

    // ... the client can now use the serverEnpoint to connect to the server

    return 0;
}
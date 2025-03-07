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

    // Create an IP to represent the server IP address that the client
    // is goint to connect to.
    boost::asio::ip::address serverIPAddress =
        boost::asio::ip::make_address(argv[1]);

    // Get the server port number.
    std::size_t portNumber = std::atoi(argv[2]);
    
    // Create a TCP endpoint in the provided IP and portNumber.
    boost::asio::ip::tcp::endpoint serverEndpoint(
        serverIPAddress, portNumber);

    std::cout << "Client can connect to: " << serverEndpoint << "\n";

    // ... the client can now use the serverEnpoint to connect to the server

    return EXIT_SUCCESS;
}
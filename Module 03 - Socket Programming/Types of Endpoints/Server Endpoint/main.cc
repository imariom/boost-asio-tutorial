#include <iostream>

#include <boost/asio.hpp>

int main(int argc, char** argv)
{
    // Check command line arguments.
    if (argc != 3)
    {
        std::cerr <<
            "Usage: server <server-ip-address> <port-number>\n" <<
            "Example:\n" <<
            "   server 127.0.0.1 54321\n";
        return EXIT_FAILURE;
    }

    // Create an IP to represent the server IP address.
    boost::asio::ip::address ipAddress =
        boost::asio::ip::make_address(argv[1]);

    // Get the server port number.
    std::size_t portNumber = std::atoi(argv[2]);
    
    // Create a TCP endpoint in the provided IP and portNumber.
    boost::asio::ip::tcp::endpoint endpoint(
        ipAddress, portNumber);

    std::cout << "Server endpoint: " << endpoint << std::endl;

    // ... the endpoint can be used by the server to listen
    // for incomming connections.

    return EXIT_SUCCESS;
}
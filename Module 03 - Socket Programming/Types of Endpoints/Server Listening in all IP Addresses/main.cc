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

    // Get the server port number.
    std::size_t portNumber = std::atoi(argv[1]);

    // Represent any IP address in the host machine
    boost::asio::ip::address anyIPv4 = boost::asio::ip::address_v4::any();
    boost::asio::ip::address anyIPv6 = boost::asio::ip::address_v6::any();
    
    // Create a TCP endpoint for the server for all host IPv4 addresses
    // in a provided portNumber.
    boost::asio::ip::tcp::endpoint endpoint(anyIPv4, portNumber);

    std::cout << "Server endpoint: " << endpoint << std::endl;

    // ... the endpoint can be used by the server to listen
    // for incomming connections.

    return EXIT_SUCCESS;
}
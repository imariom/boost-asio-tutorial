#include <iostream>
#include <array>
#include <vector>
#include <string>

#include <boost/asio.hpp>

int main(int argc, char** argv)
{
    // Check command line arguments
    if (argc != 3)
    {
        std::cerr <<
        "Usage: client <server-ip-address> <port-number>\n" <<
        "Example:\n" <<
        "   client 127.0.0.1 5432\n";
        return EXIT_FAILURE;
    }

    // Create the I/O execution context
    boost::asio::io_context ioCtx;

    // Create an active socket
    boost::asio::ip::tcp::socket socket(ioCtx);

    try
    {
        // Create the server endpoint
        boost::asio::ip::tcp::endpoint endpoint(
            boost::asio::ip::make_address(argv[1]), std::atoi(argv[2]));
        
        // Connect to the server
        socket.connect(endpoint);
        std::cout << "[CONNECTED TO] " << endpoint << '\n';

        // Create and fill the buffer with data
        char message[1024] = "Hello from the client!";
        // const std::string message = "Hello from the client!";

        // Send data to the server
        boost::asio::write(socket, boost::asio::buffer(message));
        std::cout << "[SENT] " << message << '\n';

        // Prepare the buffer to receive data
        char data[1024];
        // std::vector<char> data(1024);
        // std::array<char, 1024> data;

        // Receive a data from the server
        std::size_t length = socket.read_some(boost::asio::buffer(data));
        std::cout << "[RECEIVED] " << std::string(data, length) << '\n';
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
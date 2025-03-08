#include <iostream>
#include <array>
#include <vector>

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

    try
    {
        // Create the endpoint
        boost::asio::ip::tcp::endpoint endpoint(
            boost::asio::ip::tcp::v4(), std::atoi(argv[1]));

        // Create the passive socket
        boost::asio::ip::tcp::acceptor acceptor(ioCtx, endpoint);
        std::cout << "Server is listening for connection requests at " << endpoint << "\n";

        // Create an active socket that represent the client connection/session
        boost::asio::ip::tcp::socket socket(ioCtx);

        // Wait and process the next connection request in the active socket.
        acceptor.accept(socket);
        std::cout << "[ACCEPTED] client " << socket.remote_endpoint() << '\n';

        // Receive a message from the client
        // char reply[1024];
        // std::array<char, 1024> reply;
        std::vector<char> reply(1024);
        size_t length = socket.read_some(boost::asio::buffer(reply));
        std::cout << "[RECEIVED] " << std::string(reply.data(), length) << '\n';

        // Send a welcome message to the client
        // const char message[1024] = "Hello from the server!";
        const std::string message = "Hello from the server!";
        boost::asio::write(socket, boost::asio::buffer(message));
        std::cout << "[SENT] " << message << '\n';
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
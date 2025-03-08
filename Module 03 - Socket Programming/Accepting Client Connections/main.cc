#include <iostream>

#include <boost/asio.hpp>

int main(int argc, char** argv)
{
    // Check command line arguments.
    if (argc != 2 && argc != 3)
    {
        std::cerr <<
            "Usage: server <port-number> <backlog-size>\n" <<
            "Example:\n" <<
            "   server 54321 50\n";
        return EXIT_FAILURE;
    }

    // backlogSize represent the number of connection requests
    // in the host OS queue of connections.
    std::size_t backlogSize =
        (argc == 3 && std::atoi(argv[2]) != 0) ?  std::atoi(argv[2]) : 30;
    
    // Create the I/O execution context
    boost::asio::io_context ioCtx;

    // Create the endpoint
    boost::asio::ip::tcp::endpoint endpoint(
        boost::asio::ip::tcp::v4(), std::atoi(argv[1]));

    try
    {
        // Create the passive socket
        boost::asio::ip::tcp::acceptor acceptor(ioCtx, endpoint.protocol());

        // Bind the acceptor to an endpoint
        acceptor.bind(endpoint);

        // Listen for incomming connection requests.
        acceptor.listen(backlogSize);
        std::cout << "Server is listening for connection requests at " << endpoint << "\n";

        // Create an active socket that represent the client connection/session
        boost::asio::ip::tcp::socket socket(ioCtx);

        // Wait and process the next connection request in the active socket.
        acceptor.accept(socket);

        std::cout << "[ACCEPTED] client " << socket.remote_endpoint() << '\n';

        // ... at this point the 'socket' is connected to the
        // client and can be used to send data to or receive data from it.

        // Send a welcome message to the client
        std::string message = "Hello from the server!";
        boost::asio::write(socket, boost::asio::buffer(message));
        std::cout << "[SENT] " << message << '\n';

        // Receive a message from the client
        char reply[1024];
        size_t length = socket.read_some(boost::asio::buffer(reply));
        std::cout << "[RECEIVED] " << std::string(reply, length) << '\n';
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
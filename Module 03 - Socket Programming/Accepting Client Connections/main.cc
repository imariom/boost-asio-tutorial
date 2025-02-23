#include <iostream>
#include <string>

#include <boost/asio.hpp>

int main(int argc, char* argv[])
{
    // Get server port number and backlog from the command line arguments
    if (argc < 3)
    {
        std::cerr << "Usage: server <port-number> <backlog>\n";
        return -1;
    }
    std::size_t portNumber = std::atoi(argv[1]);

    // backlogSize represent the number of connection requests
    // in the host OS queue of connections.
    std::size_t backlogSize = std::atoi(argv[2]);
    if (backlogSize <= 0) {
        backlogSize = 30;
    }
    
    // Create the I/O execution context
    boost::asio::io_context ioCtx;

    // Create the passive socket
    boost::asio::ip::tcp::acceptor acceptor(ioCtx);

    try
    {
        // Bind the acceptor to an endpoint
        acceptor.bind(boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),
            portNumber));

        // Listen for incomming connection requests.
        acceptor.listen(backlogSize);
        std::cout << "Server is listening on 0.0.0.0:" << portNumber << "...\n";

        // Create an active socket that reqpresent the client connection/session
        boost::asio::ip::tcp::socket socket(ioCtx);

        // Wait and process the next connection request in the active socket.
        acceptor.accept(socket);

        std::cout << "Client connected!" << std::endl;

        // ... at this point the 'socket' is connected to the
        // client and can be used to send data to or receive data from it.

        // Send a welcome message to the client
        std::string message = "Hello from the server!";
        boost::asio::write(socket, boost::asio::buffer(message));

        // Receive a message from the client
        char reply[1024];
        size_t length = socket.read_some(boost::asio::buffer(reply));
        std::cout << "Client says: " << std::string(reply, length) << std::endl;
    }
    catch (std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
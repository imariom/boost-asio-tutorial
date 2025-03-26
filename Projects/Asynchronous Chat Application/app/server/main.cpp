#include <iostream>

#include <chatito.hpp>

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cerr <<
            "Usage: chatito <address> <port>\n" <<
            "   For IPv4, try:\n" <<
            "       chatito 0.0.0.0 80\n" <<
            "   For IPv6, try:\n" <<
            "       chatito 0::0 80\n";
        return EXIT_FAILURE;
    }
    
    try
    {
        // Initialise the server.
        chatito::Server server(argv[1], argv[2]);

        // Run the server until stopped.
        server.run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
#include <iostream>

#include <boost/asio.hpp>

std::string extractScheme(const std::string& host)
{
    // Find the position of "://"
    auto pos = host.find("://");
    if (pos == std::string_view::npos)
    {
        // No scheme found, return default or throw an error
        return "http"; // Default to HTTP if no scheme is present
    }

    // Extract the scheme (substring before "://")
    std::string scheme = host.substr(0, pos);

    // Validate the scheme
    if (scheme == "http" || scheme == "https")
    {
        return scheme;
    }
    else
    {
        throw std::invalid_argument("Invalid scheme in URL: " + std::string(host));
    }
}

int main(int argc, char* argv[])
{
    // Get client host name from the command line arguments
    if (argc < 2)
    {
        std::cerr << "Usage: client <host>\n";
        return -1;
    }
    std::string hostName = argv[1];
    std::string scheme = extractScheme(argv[2]);
    
    // Create the I/O execution context
    boost::asio::io_context ioCtx;

    // Create a resolver
    boost::asio::ip::tcp::resolver resolver(ioCtx);

    try
    {
        // Resolve the DNS name (e.g., "www.example.com") and service (e.g., "http")
        boost::asio::ip::tcp::resolver::results_type endpoints =
            resolver.resolve(hostName, scheme);

        // Iterate over the resolved endpoints
        for (const auto& endpoint : endpoints)
        {
            std::cout << "Resolved endpoint: "
                << endpoint.endpoint() << std::endl;
        }

        // Use the first endpoint to create a socket and connect
        boost::asio::ip::tcp::socket socket(ioCtx);
        boost::asio::connect(socket, endpoints);

        std::cout << "Connected to: "
            << socket.remote_endpoint() << std::endl;
    }
    catch (std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
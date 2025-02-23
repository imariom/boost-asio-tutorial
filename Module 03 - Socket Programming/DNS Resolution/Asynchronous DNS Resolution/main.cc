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
    // Get server host name from the command line arguments
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

    // Asynchronously resolve the DNS name
    resolver.async_resolve(hostName, scheme,
        [&](const boost::system::error_code& ec,
            boost::asio::ip::tcp::resolver::results_type endpoints)
            {
                if (ec)
                {
                    std::cerr << "Error resolving DNS: "
                        << ec.message() << std::endl;
                    return;
                }

                // Iterate over the resolved endpoints
                for (const auto& endpoint : endpoints)
                {
                    std::cout << "Resolved endpoint: "
                        << endpoint.endpoint() << std::endl;
                }

            // Use the first endpoint to create a socket and connect
            auto socket = std::make_shared<boost::asio::ip::tcp::socket>(ioCtx);
            boost::asio::async_connect(*socket, endpoints,
                [socket](const boost::system::error_code& ec,
                    const boost::asio::ip::tcp::endpoint& endpoint) {
                    if (ec)
                    {
                        std::cerr << "Error connecting: "
                            << ec.message() << std::endl;
                        return;
                    }

                    std::cout << "Connected to: "
                        << socket->remote_endpoint() << std::endl;
                });
        });

    // Run the io_context to start the asynchronous operation
    ioCtx.run();

    return 0;
}
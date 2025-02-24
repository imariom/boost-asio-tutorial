#include <iostream>
#include <thread>
#include <chrono>

#include <boost/asio.hpp>

int main()
{
    using namespace std::chrono_literals;
    
    // Create the I/O execution context
    boost::asio::io_context ioCtx;

    try
    {
        // Create a server endpoint (listening on port 54321)
        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), 54321);
        boost::asio::ip::tcp::acceptor acceptor(ioCtx, endpoint);

        std::cout << "Server is listening on " << endpoint << std::endl;

        // Create a socket
        boost::asio::ip::tcp::socket socket(ioCtx);

        boost::asio::steady_timer timer(ioCtx, 10s);



        // Accept a connection asynchronously
        acceptor.async_accept(socket, [&](const boost::system::error_code& ec) {
            if (ec == boost::asio::error::operation_aborted)
            {
                std::cout << "Accept operation canceled!" << std::endl;
            }
            else if (ec)
            {
                std::cerr << "Accept error: " << ec.message() << std::endl;
            }
            else
            {
                char data[1024];
                socket.async_read_some(boost::asio::buffer(data),
                    [](const boost::system::error_code& ec, std::size_t)
                    {

                    });
            }
        });

        // Run the IO context in a separate thread
        std::thread ioThread([&]() { ioCtx.run(); });

        // Simulate some work
        std::this_thread::sleep_for(std::chrono::seconds(2));

        // Wait for the IO thread to finish
        ioThread.join();
    }
    catch (std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
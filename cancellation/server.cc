#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <chrono>

using boost::asio::ip::tcp;

int main() {
    try {
        boost::asio::io_context io_context;

        // Create a server endpoint (listening on port 12345)
        tcp::endpoint endpoint(tcp::v4(), 12345);
        tcp::acceptor acceptor(io_context, endpoint);

        std::cout << "Server is listening on " << endpoint << std::endl;

        // Create a socket
        tcp::socket socket(io_context);

        // Create a cancellation signal
        boost::asio::cancellation_signal cancel_signal;

        // Accept a connection asynchronously
        acceptor.async_accept(socket, [&](boost::system::error_code ec) {
            if (ec == boost::asio::error::operation_aborted) {
                std::cout << "Accept operation canceled!" << std::endl;
            } else if (ec) {
                std::cerr << "Accept error: " << ec.message() << std::endl;
            } else {
                std::cout << "Client connected!" << std::endl;
            }
        });

        // Associate the cancellation signal with the acceptor
        acceptor.async_wait(tcp::acceptor::wait_read, [&](boost::system::error_code ec) {
            if (ec == boost::asio::error::operation_aborted) {
                std::cout << "Wait operation canceled!" << std::endl;
            }
        });

        // Run the IO context in a separate thread
        std::thread io_thread([&]() {
            io_context.run();
        });

        // Simulate some work
        std::this_thread::sleep_for(std::chrono::seconds(2));

        // Cancel the accept operation
        std::cout << "Canceling accept operation..." << std::endl;
        cancel_signal.emit(boost::asio::cancellation_type::terminal);

        // Wait for the IO thread to finish
        io_thread.join();
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
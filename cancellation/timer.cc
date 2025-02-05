#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    try {
        boost::asio::io_context io_context;

        // Create a cancellation signal
        boost::asio::cancellation_signal cancel_signal;

        // Create a timer
        boost::asio::steady_timer timer(io_context, std::chrono::seconds(5));

        // Start an asynchronous wait on the timer
        timer.async_wait([&](boost::system::error_code ec) {
            if (ec == boost::asio::error::operation_aborted) {
                std::cout << "Timer canceled!" << std::endl;
            } else if (ec) {
                std::cerr << "Timer error: " << ec.message() << std::endl;
            } else {
                std::cout << "Timer expired!" << std::endl;
            }
        });

        // Run the IO context in a separate thread
        std::thread io_thread([&]() {
            io_context.run();
        });

        // Simulate some work
        std::this_thread::sleep_for(std::chrono::seconds(2));

        // Cancel the timer
        std::cout << "Canceling timer..." << std::endl;
        timer.cancel();

        // Wait for the IO thread to finish
        io_thread.join();
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
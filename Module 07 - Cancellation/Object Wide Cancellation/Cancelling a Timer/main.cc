#include <iostream>
#include <thread>
#include <chrono>

#include <boost/asio.hpp>

int main()
{
    // Create the I/O execution context.
    boost::asio::io_context ioCtx;

    // Create a timer
    boost::asio::steady_timer timer(ioCtx, std::chrono::seconds(5));

    // Start an asynchronous wait on the timer
    timer.async_wait(
        [&](boost::system::error_code ec) {
            if (ec == boost::asio::error::operation_aborted) {
                std::cout << "Timer canceled!" << std::endl;
            } else if (ec) {
                std::cerr << "Timer error: " << ec.message() << std::endl;
            } else {
                std::cout << "Timer expired!" << std::endl;
            }
        });

    // Run the IO context in a separate thread
    std::thread ioThread([&]() { ioCtx.run(); });

    // Simulate some heavy work
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // Cancel the timer
    std::cout << "Canceling timer..." << std::endl;
    timer.cancel();

    // Wait for the IO thread to finish
    ioThread.join();

    return 0;
}
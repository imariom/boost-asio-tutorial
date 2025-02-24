#include <iostream>
#include <thread>
#include <chrono>

#include <boost/asio.hpp>

void async_operation_with_cancellation(boost::asio::io_context& ioCtx)
{
    using namespace std::chrono_literals;

    // Create a timer that expires after 5 seconds
    boost::asio::steady_timer timer(ioCtx, 5s);

    // Create a cancellation signal
    boost::asio::cancellation_signal cancelSignal;

    // Start an asynchronous wait on the timer
    timer.async_wait(
        boost::asio::bind_cancellation_slot(cancelSignal.slot(),
            [](const boost::system::error_code& ec)
            {
                if (ec == boost::asio::error::operation_aborted)
                {
                    std::cout << "Timer was cancelled!" << std::endl;
                }
                else if (!ec)
                {
                    std::cout << "Timer expired!" << std::endl;
                }
            }));

    // Run the IO context in a separate thread
    std::thread ioThread([&ioCtx]() { ioCtx.run(); });

    // Simulate some work in the main thread
    std::this_thread::sleep_for(2s);

    // Cancel the timer after 2 seconds
    std::cout << "Cancelling the timer..." << std::endl;
    cancelSignal.emit(boost::asio::cancellation_type::total);

    // Wait for the IO thread to finish
    ioThread.join();
}

int main()
{
    // Create the I/O execution context
    boost::asio::io_context ioCtx;

    async_operation_with_cancellation(ioCtx);

    return 0;
}
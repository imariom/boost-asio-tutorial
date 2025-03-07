#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <vector>

using boost::asio::ip::tcp;

void worker(boost::asio::io_context& io_context)
{
    std::cout << "Thread " << std::this_thread::get_id() << " started\n";
    io_context.run();
    std::cout << "Thread " << std::this_thread::get_id() << " finished\n";
}

int main()
{
    try
    {
        boost::asio::io_context io_context;

        // Create a work guard to keep the io_context running
        auto work = boost::asio::make_work_guard(io_context);

        // Create a thread pool
        std::vector<std::thread> threads;
        const int num_threads = 4;

        for (int i = 0; i < num_threads; ++i) {
            threads.emplace_back(worker, std::ref(io_context));
        }

        // Simulate some asynchronous work
        boost::asio::post(io_context, []() {
            std::cout << "Hello from the main thread!\n";
        });

        // Stop the io_context (this will cause the threads to exit)
        io_context.stop();

        // Join the threads
        for (auto& t : threads)
        {
            t.join();
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
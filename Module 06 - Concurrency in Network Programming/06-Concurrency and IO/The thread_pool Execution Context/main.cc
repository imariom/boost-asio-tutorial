#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>
#include <thread>

using boost::asio::thread_pool;

void task(int id)
{
    std::cout << "Task " << id
        << " started on thread " << std::this_thread::get_id() << '\n';

    std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate work

    std::cout << "Task " << id
        << " completed on thread " << std::this_thread::get_id() << '\n';
}

int main()
{
    // Create a thread pool with 4 threads
    thread_pool pool(4);

    // Post tasks to the thread pool
    for (int i = 0; i < 8; ++i)
    {
        boost::asio::post(pool, [i]() { task(i); });
    }

    // Wait for all tasks to complete
    pool.join();

    std::cout << "All tasks completed." << std::endl;
    return 0;
}
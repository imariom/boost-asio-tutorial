#include <iostream>
#include <thread>

#include <boost/asio.hpp>

void task(int id) {
    std::cout << "Task " << id << " started on thread " << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate work
    std::cout << "Task " << id << " completed on thread " << std::this_thread::get_id() << std::endl;
}

int main()
{
    // Get the system-wide execution context
    boost::asio::system_context context;

    // Post tasks to the system context
    for (int i = 0; i < 4; ++i)
    {
        boost::asio::post(context, [i]() { task(i); });
    }

    std::cout << "All tasks completed." << std::endl;
    std::getchar();

    return 0;
}
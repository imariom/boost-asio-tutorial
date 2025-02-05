#include <boost/asio.hpp>

int main()
{
    // Note: We can can create buffers from raw arrays 

    // Create mutable buffer for receiving
    char receive_data[1024]; // Mutable buffer for receiving data
    boost::asio::mutable_buffer recv_buffer = boost::asio::buffer(receive_data, sizeof(receive_data));

    // Create const buffer for sending
    char send_data[] = "Hello, world!"; // Const buffer for sending data
    boost::asio::const_buffer send_buffer = boost::asio::buffer(send_data, sizeof(send_data));
}
#include <boost/asio.hpp>

int main()
{
    // Note: We can create buffers from std::array

    // Create mutable buffer for receiving
    std::array<char, 1024> receive_data; // Mutable buffer for receiving data
    boost::asio::mutable_buffer recv_buffer = boost::asio::buffer(receive_data);

    // Create const buffer for sending
    std::array<char, 14> send_data = {"Hello, world!"}; // Const buffer for sending data
    boost::asio::const_buffer send_buffer = boost::asio::buffer(send_data);
}
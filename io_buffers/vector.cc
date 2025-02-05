#include <boost/asio.hpp>

int main()
{
    // Note: We can create buffers from std::vector 

    // Create mutable buffer for receiving
    std::vector<char> receive_data(1024); // Mutable buffer for receiving data
    boost::asio::mutable_buffer recv_buffer = boost::asio::buffer(receive_data);

    // Create const buffer for sending
    std::string send_data = "Hello, world!"; // Const buffer for sending data
    boost::asio::const_buffer send_buffer = boost::asio::buffer(send_data);
}
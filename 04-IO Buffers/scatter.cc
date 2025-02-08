#include <boost/asio.hpp>

int main()
{
    // Boost.Asio supports scatter-gather I/O, where you can
    // read into or write from multiple buffers in a single operation
    
    std::array<char, 128> buffer1;
    std::array<char, 256> buffer2;

    // Create a buffer sequence
    std::vector<boost::asio::mutable_buffer> buffers;
    buffers.push_back(boost::asio::buffer(buffer1));
    buffers.push_back(boost::asio::buffer(buffer2));

    // Read data into multiple buffers
    size_t length = socket.read_some(buffers);

    // Process the received data
    std::cout << "Received " << length << " bytes" << std::endl;
}
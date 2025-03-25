#include <iostream>
#include <memory>
#include <unordered_set>
#include <chrono>

#include <boost/asio.hpp>

using namespace std::chrono_literals;

class session : public std::enable_shared_from_this<session>
{
public:
    explicit session(boost::asio::ip::tcp::socket socket)
        : socket_(std::move(socket)), 
          timer_(socket.get_executor(), 5s)
    {
    }

    void start()
    {
        std::cout << "New client connected: " 
                  << socket_.remote_endpoint() << "\n";
        read_data();
        start_timeout();
    }

private:
    boost::asio::ip::tcp::socket socket_;
    boost::asio::steady_timer timer_;
    boost::asio::cancellation_signal cancelSignal_;
    char data_[128];

    void read_data()
    {
        auto self = shared_from_this();
        socket_.async_read_some(boost::asio::buffer(data_),
            boost::asio::bind_cancellation_slot(
                cancelSignal_.slot(),
                [this, self](const boost::system::error_code& ec, std::size_t length) {
                    if (!ec) {
                        std::cout << "Message received: " 
                                  << std::string(data_, length) << '\n';

                        write_data(); // echo message back
                        reset_timeout(); // Reset timer since data was received
                    } else {
                        std::cout << "Client disconnected: " << ec.message() << "\n";
                        write_data();
                    }
                })
            );
    }

    void write_data()
    {
        auto self = shared_from_this();
        socket_.async_write_some(boost::asio::buffer(data_),
            [this, self](const boost::system::error_code& ec, std::size_t length) {
                if (!ec) {
                    std::cout << "Message sent: "
                        << std::string(data_, length) << '\n';

                    read_data(); // read next message
                }
            });
    }

    void start_timeout()
    {
        auto self = shared_from_this();
        timer_.async_wait([this, self](boost::system::error_code ec) {
            if (!ec) { 
                std::cout << "Client inactive, closing connection...\n";

                // Cancel the ongoing read operation
                cancelSignal_.emit(boost::asio::cancellation_type::total);
            }
        });
    }

    void reset_timeout()
    {
        timer_.expires_after(5s); // Reset the timeout to 5s
        start_timeout();
    }

    void close()
    {
        boost::system::error_code ec;
        socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
        socket_.close(ec);
    }
};

class echo_server
{
public:
    echo_server(boost::asio::io_context& io,
        boost::asio::ip::tcp::endpoint endpoint)
        : acceptor_(io, endpoint)
    {
        accept_connections();
    }

private:
    boost::asio::ip::tcp::acceptor acceptor_;

    void accept_connections()
    {
        acceptor_.async_accept([this](boost::system::error_code ec,
            boost::asio::ip::tcp::socket socket) {
            if (!ec) {
                std::make_shared<session>(std::move(socket))->start();
            }
            accept_connections(); // Accept next client
        });
    }
};

int main(int argc, char* argv[])
{
    // Get server port number from the command line arguments.
    if (argc < 2) {
        std::cerr << "Usage: server <port-number>\n";
        return -1;
    }

    // Create the I/O execution context.
    boost::asio::io_context ioCtx;

    // Create the server endpoint through which the server
    // will accept client connections.
    boost::asio::ip::tcp::endpoint endpoint(
        boost::asio::ip::tcp::v4(), std::atoi(argv[2]));
    
    try {
        echo_server server(ioCtx, endpoint);
        ioCtx.run(); // to poll and execute asynchronous operations
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
    }

    return 0;
}
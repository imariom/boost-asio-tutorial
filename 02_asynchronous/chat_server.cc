#include <boost/asio.hpp>
#include <iostream>

using boost::asio::ip::tcp;

class AsyncClient {
public:
    AsyncClient(boost::asio::io_context& io_context, const std::string& host, const std::string& port)
        : resolver_(io_context), socket_(io_context)
    {
        resolver_.async_resolve(host, port,
            [this](const boost::system::error_code& ec, tcp::resolver::results_type results) {
                if (!ec) {
                    boost::asio::async_connect(socket_, results,
                        [this](const boost::system::error_code& ec, tcp::endpoint) {
                            if (!ec) {
                                send_message("Hello, Server!");
                            }
                        });
                }
            });
    }

private:
    void send_message(const std::string& message)
    {
        boost::asio::async_write(socket_, boost::asio::buffer(message),
            [this](const boost::system::error_code& ec, std::size_t) {
                if (!ec) {
                    receive_response();
                }
            });
    }

    void receive_response()
    {
        socket_.async_read_some(boost::asio::buffer(data_),
            [this](const boost::system::error_code& ec, std::size_t length) {
                if (!ec) {
                    std::cout << "Reply: " << std::string(data_, length) << std::endl;
                }
            });
    }

    tcp::resolver resolver_;
    tcp::socket socket_;
    char data_[1024];
};

int main() {
    try {
        boost::asio::io_context io_context;
        AsyncClient client(io_context, "127.0.0.1", "12345");
        io_context.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}
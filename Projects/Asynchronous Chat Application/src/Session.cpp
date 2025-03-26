#include <chatito/Session.hpp>
#include <chatito/SessionManager.hpp>

namespace chatito {

Session::Session(boost::asio::ip::tcp::socket socket, SessionManager& sessionManager)
    : socket_(std::move(socket)), sessionManager_(sessionManager)
{
}

void Session::start()
{
    read();
}

void Session::stop()
{
    socket_.close();
}

void Session::read()
{
    auto self = shared_from_this();
    socket_.async_read_some(buffer_,
        [this, self](const boost::system::error_code& ec,
            std::size_t bytes_transferred)
        {
            if (!ec)
            {
                
            }
            else if (ec != boost::asio::error::operation_aborted)
            {
                sessionManager_.stop(shared_from_this());
            }
        });
}

void Session::write()
{

}
    
} // namespace chatito

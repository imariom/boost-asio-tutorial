#ifndef CHATITO_CHAT_SERVER_HPP
#define CHATITO_CHAT_SERVER_HPP

#include "SessionManager.hpp"

#include <boost/asio.hpp>

namespace chatito {

class ChatServer
{
public:
    // Prohibit copying and moving
    ChatServer(const ChatServer&) = delete;
    ChatServer& operator=(const ChatServer&) = delete;
    ChatServer(ChatServer&&) = delete;
    ChatServer& operator=(ChatServer&&) = delete;

    explicit ChatServer(const std::string& address, const std::string& port);
    void run();

private:
    void accept();
    void awaitStop();

    boost::asio::io_context& ioCtx_;
    boost::asio::ip::tcp::acceptor acceptor_;
    boost::asio::signal_set signals_;
    SessionManager sessionManager_;
};
    
} // chatito

#endif // CHATITO_CHAT_SERVER_HPP
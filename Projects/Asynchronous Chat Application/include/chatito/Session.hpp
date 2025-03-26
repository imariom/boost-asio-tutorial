#ifndef CHATITO_SESSION_HPP
#define CHATITO_SESSION_HPP

#include <memory>
#include <boost/asio.hpp>

namespace chatito {

class SessionManager;
    
class Session : public std::enable_shared_from_this<Session>
{
public:
    // Prohibit copying and moving
    Session(const Session&) = delete;
    Session& operator=(const Session&) = delete;
    Session(Session&&) = delete;
    Session& operator=(Session&&) = delete;

    explicit Session(boost::asio::ip::tcp::socket socket,
        SessionManager& sessionManager);

    void start();
    void stop();

private:
    void read();
    void write();

    SessionManager& sessionManager_;
    boost::asio::ip::tcp::socket socket_;
    std::array<char, 8192> buffer_;
};

using SessionPtr = std::shared_ptr<Session>;
    
} // namespace chatito


#endif // CHATITO_SESSION_HPP
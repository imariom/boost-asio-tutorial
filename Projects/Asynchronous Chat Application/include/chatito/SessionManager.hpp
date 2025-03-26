#ifndef CHATITO_SESSION_MANAGER_HPP
#define CHATITO_SESSION_MANAGER_HPP

#include "Session.hpp"

#include <set>

namespace chatito {

class SessionManager
{
public:
    // Prohibit copying and moving
    SessionManager(SessionManager&) = delete;
    SessionManager& operator=(SessionManager&) = delete;
    SessionManager(SessionManager&&) = delete;
    SessionManager& operator=(SessionManager&&) = delete;

    SessionManager();

    /// @brief Add the specified session to the manager and start it.
    /// @param sessionPtr The session to manage.
    void start(SessionPtr sessionPtr);
    
    /// @brief Stop the specified session.
    /// @param sessionPtr The session to be stopped.
    void stop(SessionPtr sessionPtr);

    /// @brief Stop all sessions.
    void stopAll();

private:
    /// @brief The managed sessions.
    std::set<SessionPtr> sessions_;
};
    
} // namespace chatito


#endif // CHATITO_SESSION_MANAGER_HPP
#include <chatito/SessionManager.hpp>

namespace chatito {
    
SessionManager::SessionManager()
{
}

void SessionManager::start(SessionPtr session)
{
    sessions_.insert(session);
    session->start();
}

void SessionManager::stop(SessionPtr session)
{
    sessions_.erase(session);
    session->stop();
}

void SessionManager::stopAll()
{
    for (auto session : sessions_)
        session->stop();
    sessions_.clear();
}

} // namespace chatito

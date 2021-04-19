#include "shared_data.h"
#include "Ws_session.h"

Shared_data::Shared_data()
{

}

void Shared_data::addSession(std::string_view name, WsSession *ws)
{
    sessionsPool.insert(std::make_pair(name, ws));
}

void Shared_data::renameSession(const std::string &oldName, const std::string &newName)
{
    if(auto itr = sessionsPool.find(oldName); itr != sessionsPool.end())
    {
        sessionsPool[newName] = itr->second;
        sessionsPool.erase(oldName);
    }
}

void Shared_data::broadCast(std::string message)
{
    for(auto& [name, ws]: sessionsPool)
    {
        std::lock_guard lg(mutex);
        ws->do_write(message);
    }
}

void Shared_data::removeSession(const std::string name, WsSession*)
{
    sessionsPool.erase(name);
}

bool Shared_data::nameAvailable(const std::string& name)
{
    if(name.empty())
        return false;
    return (sessionsPool.find(name) != sessionsPool.end());
}

#include "shared_data.h"
#include "session.h"

Shared_data::Shared_data()
{

}

void Shared_data::addSession(std::string_view name, Session *ws)
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
    std::cout << "[!] Broadcasting message: "<< message << std::endl;
    for(auto& [name, session]: sessionsPool)
    {
        std::lock_guard lg(mutex);
        session->doAsyncWrite(message);
    }
}

void Shared_data::removeSession(const std::string name, Session*)
{
    sessionsPool.erase(name);
}

bool Shared_data::nameAvailable(const std::string& name)
{
    if(name.empty())
        return false;
    return (sessionsPool.find(name) != sessionsPool.end());
}

std::optional<Session *> Shared_data::getByUsername(const std::string &username)
{
    if(auto itr = sessionsPool.find(username); itr != sessionsPool.end())
        return std::make_optional<Session*>(itr->second);
    return std::nullopt;
}

std::map<std::string, Session *> &Shared_data::getSessionPool()
{
    return sessionsPool;
}

Shared_data* Shared_data::get()
{
    if(!instance)
        instance = new Shared_data();
    return instance;

}


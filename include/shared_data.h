#ifndef SHARED_DATA_H
#define SHARED_DATA_H

#include <map>
#include <string_view>
#include <mutex>

class WsSession;

class Shared_data
{
    std::map<std::string, WsSession*> sessionsPool;
    std::mutex mutex;
public:
    Shared_data();
    void addSession(std::string_view name, WsSession* ws);
    void renameSession(const std::string &oldName, const std::string &newName);
    void broadCast(std::string message);
    void removeSession(const std::string name, WsSession *ws);
    bool nameAvailable(const std::string &name);
};

#endif // SHARED_DATA_H

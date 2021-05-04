#ifndef SHARED_DATA_H
#define SHARED_DATA_H

#include <memory>
#include <map>
#include <string_view>
#include <mutex>

class Session;

class Shared_data
{
    std::map<std::string, Session *> sessionsPool;
    std::mutex mutex;
public:
    Shared_data();
    void addSession(std::string_view name, Session* ws);
    void renameSession(const std::string &oldName, const std::string &newName);
    void broadCast(std::string message);
    void removeSession(const std::string name, Session *ws);
    bool nameAvailable(const std::string &name);
    std::optional<Session *> getByUsername(const std::string &username);
    std::map<std::string, Session *> &getSessionPool();

    static Shared_data* get();
    static Shared_data* instance;
};

#endif // SHARED_DATA_H

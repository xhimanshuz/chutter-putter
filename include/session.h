#ifndef SESSION_H
#define SESSION_H

#include <iostream>
#include <exception>
#include <boost/asio.hpp>
#include <boost/json.hpp>
#include <boost/json/parse.hpp>

enum ASK_PEER
{
    REJECTED = 0,
    ACCEPTED = 1,
    WAITING = 2
};

class Shared_data;

namespace net = boost::asio;

class Session: public std::enable_shared_from_this<Session>
{
    std::string public_ip;
    uint16_t public_port;
    std::string private_ip;
    uint16_t private_port;
    std::string username;

    net::ip::tcp::socket _socket;
    Shared_data *sharedData;
    boost::system::error_code ec;                   // FOR SYNC CALLS
    char _buffer[1024];

    boost::json::object toJson(const std::string& buff);
    boost::json::object toJson();
    void handleRequest(boost::json::object request);
    void handleResponse(boost::json::object response);
    void doAsyncRead();
    void onRead(boost::system::error_code ec, size_t size);
    void doWrite(const std::string& buffer);
    void onWrite(boost::system::error_code ec, size_t size);
    void downGradeAccept(boost::json::object &request, std::optional<boost::json::object> &response);
    boost::json::object createSuccessResponse(const std::string& responseType);
    void createSuccessResponse(std::optional<boost::json::object> &response, const std::string& responseType);

    boost::json::object createUnSuccessResponse(const std::string& responseType, const std::string &message);
    void createUnSuccessResponse(std::optional<boost::json::object> &response, const std::string& responseType, const std::string &message);

    std::string jsonToString(boost::json::object &json);

    void setPeerData(boost::json::object &request, std::optional<boost::json::object> &response);
    void getClientList(boost::json::object &request, std::optional<boost::json::object> &response);
    const std::string getClientList();
    void onAskPeer(boost::json::object &request, std::optional<boost::json::object> &response);
    void doAskPeer(boost::json::object &response);
    void doAskPeerAccept(boost::json::object &response);
    void doAskPeerReject(boost::json::object &response);
    void createAskPeerResponse(std::optional<boost::json::object> &response, const std::string &status, const std::string description);
    void askToAccept(Session *peers);
    void sendToUserName(const std::string &username, boost::json::object &json);
    void sendToPeer(boost::json::object &json);
    inline void cleanBuffer();
public:
    void doAsyncWrite(const std::string& buffer);
    Session(net::ip::tcp::socket socket);
    virtual ~Session();
    void run(const std::string& buff);
};

#endif // SESSION_H2

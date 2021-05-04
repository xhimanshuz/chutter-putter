#include "session.h"
#include "shared_data.h"
Shared_data* Shared_data::instance = nullptr;

Session::Session(boost::asio::ip::tcp::socket socket):
    public_ip{socket.remote_endpoint().address().to_string()}, public_port{socket.remote_endpoint().port()},
    private_ip{""}, private_port{0}, username{""}, _socket{std::move(socket)}
{

}

boost::json::object Session::toJson(char *buff)
{
    auto json = boost::json::parse(buff).as_object();
    auto requset = json.at("type").as_string().data();
    return json;
}

boost::json::object Session::toJson()
{
    boost::json::object json;
    try {
        json = boost::json::parse(_buffer).as_object();
        cleanBuffer();
    }
    catch(std::exception& ex) {
        std::cout << "[x] Exception occured while parsing to Json, " << ex.what()<<std::endl;
    }
    return json;
}

void Session::handleRequest(boost::json::object request)
{
    std::string request_type = "unknown_request";
    std::optional<boost::json::object> response = std::make_optional<boost::json::object>();
    try
    {
        request_type = request["request"].as_string().data();

        if(request_type == "downgrade") {
            downGradeAccept(request, response);
        }
        else if(request_type == "clientList") {
            getClientList(request, response);
        }
        else if(request_type == "setPeerData") {
            setPeerData(request, response);
        }
        else if(request_type == "setPrivateEndpoint") {

        }
        else if(request_type == "setUsername") {

        }
        else if(request_type == "askPeer") {
            onAskPeer(request, response);
        }
        else
            throw "Invalid Request type" + request_type;

        if(response == std::nullopt)
            response = createUnSuccessResponse(request_type, "Invalid Response");
    }
    catch(std::exception& ex) {
        createUnSuccessResponse(response, request_type, ex.what());
    }

    doAsyncWrite(jsonToString(*response));

}

void Session::handleResponse(boost::json::object response)
{
    std::string response_type = "unknown_response";
    std::optional<boost::json::object> request = std::make_optional<boost::json::object>();
    try {
        response_type = response["response"].as_string().data();

        if(response_type == "askPeer") {
            doAskPeer(response);
        }

    }
    catch(std::exception& ex) {
        createUnSuccessResponse(request, response_type, ex.what());
    }

}

void Session::doAsyncRead()
{
    _socket.async_read_some(net::buffer(_buffer, sizeof(_buffer)), std::bind(&Session::onRead, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
}

void Session::onRead(boost::system::error_code ec, size_t size)
{
    if(ec)
        throw "Error Occured: " + ec.message();

    auto json = toJson();
    auto type = json["type"].get_string();
    if(type == "request")
        handleRequest(json);
    else if(type == "response")
        handleResponse(json);
    else
        std::cerr << "[x] Invlaid type"<<std::endl;

    doAsyncRead();
}

void Session::doWrite(const std::string &buffer)
{
    auto size = _socket.write_some(net::buffer(buffer.data(), buffer.size()), ec);
    onWrite(ec, size);
}

void Session::onWrite(boost::system::error_code ec, size_t size)
{
    if(!ec) {
        std::cout <<"[!] Writed bytes: "<< size <<std::endl;
    }
    else {
        throw "Error Occured: " + ec.message();
    }
}

void Session::downGradeAccept(boost::json::object &request, std::optional<boost::json::object> &response)
{
    private_ip = request["private_ip"].as_string().data();
    private_port = request["private_port"].as_int64();
    username = request["username"].as_string().data();
    public_ip = _socket.remote_endpoint().address().to_string();
    public_port = _socket.remote_endpoint().port();

    createSuccessResponse(response, "downgrade");
}

void Session::doAsyncWrite(const std::string &buffer)
{
    _socket.async_write_some(net::buffer(buffer.data(), buffer.size()), std::bind(&Session::onWrite, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
}


boost::json::object Session::createSuccessResponse(const std::string &responseType)
{
    boost::json::object json;
    json["response"] = responseType;
    json["data"] = true;
    json["error"] = false;
    json["message"] = "successful";

    return json;
}

void Session::createSuccessResponse(std::optional<boost::json::object> &response, const std::string &responseType)
{
    response->emplace("response", responseType);
    response->emplace("data", true);
    response->emplace("error", false);
    response->emplace("message", "successful");
}

boost::json::object Session::createUnSuccessResponse(const std::string &responseType, const std::string &message)
{
    boost::json::object json;
    json["response"] = responseType;
    json["data"] = false;
    json["error"] = true;
    json["message"] = message;

    return json;
}

void Session::createUnSuccessResponse(std::optional<boost::json::object> &response, const std::string &responseType, const std::string &message)
{
    response->emplace("response", responseType);
    response->emplace("data", false);
    response->emplace("error", true);
    response->emplace("message", message);
}

std::string Session::jsonToString(boost::json::object &json)
{
    return boost::json::serialize(json);
}

void Session::setPeerData(boost::json::object &request, std::optional<boost::json::object> &response)
{
    private_ip = request["private_ip"].as_string().data();
    private_port = request["private_port"].as_int64();
    username = request["username"].as_string().data();

    createSuccessResponse(response, request["request"].as_string().data());
}

void Session::getClientList(boost::json::object &request, std::optional<boost::json::object> &response)
{

    createSuccessResponse(response, request["request"].as_string().c_str());

    boost::json::object clients;
    boost::json::array clients_array;
    for(auto& [name, session]: sharedData->getSessionPool())
    {
        boost::json::object client;
        client["name"] = session->username;
        client["public_ip"] =  session->public_ip;
        client["public_port"] = session->public_port;
        client["private_ip"] = session->private_ip;
        client["private_port"] = session->private_port;

        clients_array.emplace_back(client);
    }
    clients["clients"] = clients_array;
    response->at("data") = clients;
}

void Session::onAskPeer(boost::json::object &request, std::optional<boost::json::object> &response)
{
    createSuccessResponse(response, request["request"].as_string().data());

    if(auto sesson = sharedData->getByUsername(request["username"].as_string().data()))
    {
        sesson.value()->askToAccept(this);
    }
    else
    {
        std::string message{"Username: "};
        message.append(request["username"].as_string().data()).append(" Not found");
        throw message;
    }
}

void Session::doAskPeer(boost::json::object &response)
{
    auto data = response["data"].get_object();
    switch (data["status"].get_int64())
    {
    case ASK_PEER::WAITING:
        break;
    case ASK_PEER::REJECTED:
        doAskPeerReject(response);
        break;
    case ASK_PEER::ACCEPTED:
        doAskPeerAccept(response);
        break;
    default:
        break;
    }
}

void Session::doAskPeerAccept(boost::json::object &response)
{
    auto peer_data = response["data"].get_object()["peer_data"].get_object();
    std::string t_username = peer_data["username"].as_string().c_str();
    peer_data["username"] = username;
    peer_data["private_ip"] = private_ip;
    peer_data["private_port"] = private_port;
    peer_data["public_ip"] = public_ip;
    peer_data["public_port"] = public_port;
    response["data"].get_object()["peer_data"] = peer_data;

    sendToUserName(t_username, response);
}

void Session::doAskPeerReject(boost::json::object &response)
{


}

void Session::createAskPeerResponse(std::optional<boost::json::object> &response, const std::string &status, const std::string description)
{
    boost::json::object data;
    data["status"] = status;
    data["description"] = description;
    response->at("data") = data;
}

void Session::askToAccept(Session* peer)
{
    boost::json::object json;
    json["type"] = "request";
    json["request"] = "askToAccept";
    json["private_ip"] =  peer->private_ip;
    json["private_port"] = peer->private_port;
    json["public_ip"] = peer->public_ip;
    json["public_port"] = peer->public_port;
    json["username"] = peer->username;

    auto serial = boost::json::serialize(json);
    doAsyncWrite(serial);
}

void Session::sendToUserName(const std::string &username, boost::json::object &json)
{
    if(auto session = sharedData->getByUsername(username); session) {
        session.value()->sendToPeer(json);
    }
    else {
        throw "[x] Username not found while sendToUserName, " + username;
    }
}

void Session::sendToPeer(boost::json::object &json)
{
    doAsyncWrite(jsonToString(json));
}


inline void Session::cleanBuffer()
{
    memset(&_buffer, '\0', sizeof(_buffer));
}


Session::~Session()
{
    std::cout <<"[~] Session Closed: "<< username << std::endl;
    _socket.close();
}

void Session::run(char *buff)
{
    try {
        sharedData = Shared_data::get();
        handleRequest(toJson(buff));

        sharedData->addSession(username, this);
        doAsyncRead();
    }
    catch (std::exception& ex)
    {
        std::cerr<<"Excption Occued: "<< ex.what() <<std::endl;
        auto json = createUnSuccessResponse("root_request", ex.what());
        doAsyncWrite(jsonToString(json));
    }
}

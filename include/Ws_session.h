#ifndef WS_SESSION_H
#define WS_SESSION_H

#include <iostream>
#include <boost/beast.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/http.hpp>

class Shared_data;

class WsSession: public std::enable_shared_from_this<WsSession>
{
    std::string _name;
    boost::beast::flat_buffer buffer;
    std::shared_ptr<Shared_data> _shared_data;
    boost::beast::websocket::stream<boost::asio::ip::tcp::socket> ws;
    void do_read();
    void on_read();
    void on_write();
    void do_accept();
    void registerName();
public:
    WsSession(std::shared_ptr<Shared_data> shared_data, boost::asio::ip::tcp::socket stream);
    ~WsSession();
    void run(boost::beast::http::request<boost::beast::http::string_body> _request);
    void do_write(std::string sv);
};

#endif // WS_SESSION_H

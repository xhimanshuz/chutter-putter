#ifndef HTTP_SESSION_H
#define HTTP_SESSION_H

#include <iostream>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/json/parse.hpp>

//#include "Ws_session.h"
#include "session.h"


namespace net = boost::asio;
namespace http = boost::beast::http;


class HttpSession: public std::enable_shared_from_this<HttpSession>
{
    boost::beast::tcp_stream _stream;
    boost::beast::flat_buffer _buffer;
    http::request<http::string_body> _request;
    boost::system::error_code ec;

    void do_read();
    void do_write(http::response<http::string_body> _response);
    http::response<http::string_body> handleRequest();
    http::response<http::string_body> post();
    http::response<http::string_body> get();
    http::response<http::string_body> other();
    http::response<http::string_body> createResponse(http::status status, std::string message, http::verb request_method);

    void rawSocketHandler();

public:
    explicit HttpSession(net::ip::tcp::socket socket);
    ~HttpSession();
    void run();

};

#endif // HTTP_SESSION_H

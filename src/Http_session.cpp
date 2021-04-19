#include "Http_session.h"

HttpSession::HttpSession(boost::asio::ip::tcp::socket socket, std::shared_ptr<Shared_data> sdata): _stream{std::move(socket)}, _sdata{sdata}
{
    std::cout << "[!] HTTP Session Created, "<< _stream.socket().remote_endpoint() << std::endl;
}

void HttpSession::do_read()
{
    std::cout << __FUNCTION__ << "()" << std::endl;
    http::async_read(_stream, _buffer, _request, [self = shared_from_this(), this](boost::system::error_code ec, size_t){
        if(ec)
        {
            if(ec == http::error::end_of_stream)
            {
                std::cout << "[!] Connection Closed" << std::endl;
                return;
            }
            std::cout << "Error in reading Socket: "<< ec.message()<< std::endl;
            self->do_read();
        }
        if(boost::beast::websocket::is_upgrade(self->_request))
        {
            std::cout <<"[!] WebSocket Upgrade" << std::endl;
            std::make_shared<WsSession>(std::move(_sdata), _stream.release_socket())->run(std::move(_request));
            return;
        }

        self->do_write(self->handleRequest());
    });
}

void HttpSession::do_write(http::response<http::string_body> _response)
{
    http::write(_stream, _response, ec);
    std::cout << "[!] Response Sent: \n" << _response << std::endl;

    if(ec)
        std::cout <<"[x] Error in Writing"<< ec.message() <<std::endl;
    return;
}

http::response<http::string_body> HttpSession::handleRequest()
{
    std::cout << _request << std::endl;

    switch (_request.method())
    {
        case http::verb::get:
            return get();
        case http::verb::post:
            return post();
        default:
            return other();
    }
}

http::response<http::string_body> HttpSession::post()
{
    std::cout << "POST Request Received"<< std::endl;
    return createResponse(http::status::ok, _request.body(), http::verb::post);
//    return crea
}

http::response<http::string_body> HttpSession::get()
{
    return createResponse(http::status::ok, "GET METHOD", http::verb::get);
}

http::response<http::string_body> HttpSession::other()
{
    std::cout << "OTHER Request Received"<< std::endl;
    return createResponse(http::status::method_not_allowed, "Error Invalid Method", http::verb::unknown);
}

http::response<http::string_body> HttpSession::createResponse(http::status status, std::string message, http::verb request_method)
{
    http::response<http::string_body> _response{status, _request.version()};
    if(request_method == http::verb::post)
    {
        _response.set(http::field::content_type, _request.at(http::field::content_type));
        _response.body() = message;
    }
    if(request_method == http::verb::unknown)
    {
        _response.set(http::field::content_type, "application/json");
        _response.body() = "{ \"error\" : \"Invalid Method type, Please try again\", \"code\": 420 }";
    }

    _response.prepare_payload();
    return _response;
}

HttpSession::~HttpSession()
{
//    std::cout << "[x] HTTP Session Closed, "<< _stream.socket().remote_endpoint() << std::endl;
}

void HttpSession::run()
{
    do_read();
}
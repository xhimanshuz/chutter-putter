//#include "Ws_session.h"
//#include "shared_data.h"

//WsSession::WsSession(boost::asio::ip::tcp::socket stream): ws{std::move(stream)}
//{
//    std::cout <<"[!] Webscket Session Created"<< std::endl;
//}

//void WsSession::do_read()
//{
//    ws.async_read(buffer, [self = shared_from_this(), this](boost::system::error_code ec, size_t){
//        if(ec == boost::beast::websocket::error::closed)
//            return;


//        _shared_data->broadCast(boost::beast::buffers_to_string(buffer.data()));

//        std::cout <<">> "<< boost::beast::buffers_to_string(buffer.data()) <<std::endl;

//        buffer.consume (buffer.size());
//        do_read();
//    });
//}

//void WsSession::do_write(std::string sv)
//{
//    ws.async_write(boost::asio::buffer(sv), [self = shared_from_this()](boost::system::error_code ec, size_t){
//        if(ec)
//            std::cout <<"[!] [ws] Error in Writing"<< std::endl;
//    });
//}

//void WsSession::on_read()
//{

//}

//void WsSession::on_write()
//{

//}

//void WsSession::do_accept()
//{

//    do_read();
//}

//void WsSession::registerName()
//{
//    std::string name{""};
//    do
//    {
//        ws.write(boost::asio::buffer("Please Enter username\n>> "));
//        ws.read(buffer);
//        name = boost::beast::buffers_to_string(buffer.data());
//        buffer.consume(buffer.size());
//    }
//    while(_shared_data->nameAvailable(name));
//    _name = name;
//    _shared_data->addSession(_name, this);
//    ws.write(boost::asio::buffer("User name[ "+_name+" ] Successfully Added"));

//    do_read();

//}


//WsSession::~WsSession()
//{
//    std::cout << "[x] [ws] Session Closed ";
//    _shared_data->removeSession(_name, this);
//}

//void WsSession::run(boost::beast::http::request<boost::beast::http::string_body> _request)
//{
//    ws.accept(_request);
//    registerName();
//}

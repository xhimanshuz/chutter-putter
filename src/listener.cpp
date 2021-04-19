#include "listener.h"


Listener::Listener(boost::asio::io_context &_ioc, const unsigned short port): ioc(_ioc), acceptor(ioc, net::ip::tcp::endpoint{net::ip::tcp::v4(), port}),
    shared_data(std::make_shared<Shared_data>())
{
//    std::cout << "[!] Listener Started at port: "<< port << std::endl;
    do_accept();
}

Listener::~Listener()
{

}

void Listener::do_accept()
{
    acceptor.async_accept([this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket){

        std::make_shared<HttpSession>(std::move(socket), shared_data)->run();

        do_accept();
    });
}

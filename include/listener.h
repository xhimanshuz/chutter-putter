#ifndef LISTENER_H
#define LISTENER_H

#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>

#include "Http_session.h"
#include "shared_data.h"

namespace net = boost::asio;

class Listener
{
    net::io_context& ioc;
    net::ip::tcp::acceptor acceptor;
    std::shared_ptr<Shared_data> shared_data;
public:
    Listener(net::io_context &_ioc, const unsigned short port);
    ~Listener();
    void do_accept();
};

#endif // LISTENER_H

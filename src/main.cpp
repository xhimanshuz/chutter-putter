#include <charconv>

#include "listener.h"

int main(int argc, char **argv)
{
    boost::asio::io_context ioc;

    if(argc < 2)
        throw "Please enter Port\n";

    Listener l(ioc, std::atoi(argv[1]));

    for(auto i=0;i < 2; i++)
        std::thread([&ioc]{
            ioc.run();
        }).detach();

    ioc.run();

    return 0;
}

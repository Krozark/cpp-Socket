#include <iostream>

#include <Socket/SocketSerialized.hpp>

int main(int argc, char* argv[])
{
    int port = 5001;
    if(argc < 2)
        std::cout<<"Usage is:\n"<<argv[0]<<" <msg>"<<std::endl;
    ntw::SocketSerialized sock(ntw::Socket::Dommaine::IP,ntw::Socket::Type::UDP,IPPROTO_UDP);

    sock.connect("127.255.255.255",port);
    sock.setBroadcast(true);

    sock<<argv[1];
    sock.send();
    return 0;
}

#include <iostream>
#include <Socket/SocketSerialized.hpp>


int main(int argc, char* argv[])
{
    int port = 5001;

    ntw::SocketSerialized sock(ntw::Socket::Domain::IP,ntw::Socket::Type::UDP,IPPROTO_UDP);
    sock.connect(port);
    sock.setReusable(true);
    sock.bind();

    while(true)
    {
        ntw::SocketSerialized from(ntw::Socket::Domain::IP,ntw::Socket::Type::UDP);
        from.connect(port);
        int nbBytes = sock.receive(from);
        std::cout<<"recv msg of size "<<sock.size()<<"/"<<nbBytes<<"From ip: "<<from.getIp()<<std::endl;
        std::cout<<sock<<std::endl;
    }

    return 0;
}

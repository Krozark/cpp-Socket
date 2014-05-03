#include <iostream>
#include <Socket/SocketSerialized.hpp>


int main(int argc, char* argv[])
{
    ntw::SocketSerialized sock(ntw::Socket::Dommaine::IP,ntw::Socket::Type::UDP,IPPROTO_UDP);
    sock.connect(5001);
    sock.setReusable(true);
    sock.bind();

    while(true)
    {
        int nbBytes = sock.receive();
        std::cout<<"recv msg of size "<<sock.size()<<"/"<<nbBytes<<std::endl;
        std::cout<<sock<<std::endl;
    }

    return 0;
}

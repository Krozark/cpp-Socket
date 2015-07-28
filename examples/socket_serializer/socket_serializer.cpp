#include <thread>

#include <Socket/Serializer.hpp>
#include <Socket/Socket.hpp>
#include <iostream>

int port = 5001;

void client()
{
    std::cout<<"client()"<<std::endl;

    ntw::Serializer stream;
    stream<<std::string("from client");

    ntw::Socket sock(ntw::Socket::Domain::IP,ntw::Socket::Type::UDP,IPPROTO_UDP);

    sock.connect("127.255.255.255",port);
    sock.setBroadcast(true);

    sock.send(stream);

    std::cout<<"end client()"<<std::endl;
}

void server()
{
    std::cout<<"server()"<<std::endl;

    ntw::Socket sock(ntw::Socket::Domain::IP,ntw::Socket::Type::UDP,IPPROTO_UDP);
    sock.connect(port);
    sock.setReusable(true);
    sock.bind();

    while(true)
    {
        std::cout<<"server() waiting "<<std::endl;
        ntw::Serializer stream;
        ntw::Socket from(ntw::Socket::Domain::IP,ntw::Socket::Type::UDP);
        from.connect(port);

        int nbBytes = sock.receive(stream,0,from);
        std::cout<<"server() : recv msg  <"<<stream<<">"<<nbBytes<<" From ip: "<<from.getIp()<<std::endl;
    }

    std::cout<<"end server()"<<std::endl;

}


int main(int argc,char* argv[])
{
    std::thread s(server);

    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::thread c(client);

    c.join();
    s.join();

    return 0;
}


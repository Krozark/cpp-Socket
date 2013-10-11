#include <Socket/server/Server.hpp>

#include <iostream>
#include <chrono>

/*ntw::SelectManager clientSelector;

void newclient(ntw::SelectManager& selector,ntw::Socket& sock)
{
    ntw::SocketSerialized* clientSock = new ntw::SocketSerialized(sock.accept());
    ntw::FuncWrapper::verifyConnect(*clientSock);

    //clientSock->Shutdown(ntw::Socket::Down::SEND);
    clientSelector.add(clientSock);
};

void reply(ntw::SelectManager& selector,ntw::Socket& sock)
{
    ntw::SocketSerialized& clientSock = *(ntw::SocketSerialized*)&sock;
    std::cout<<"Répond à "<<clientSock.id()<<std::endl;
    if(clientSock.receive() >0)
    {
        ntw::FuncWrapper::dispatch(clientSock);
    }
    else
    {
        std::cerr<<"Client connection lost <id:"<<clientSock.id()<<">"<<std::endl; 
        selector.remove(&clientSock);
        delete &clientSock;
    }
};*/


int main(int argc, char* argv[])
{

    /*ntw::SocketSerialized sockSer(ntw::Socket::Dommaine::IP,ntw::Socket::Type::TCP);
    sockSer.serveurMode();

    ntw::SelectManager serverSelector;
    serverSelector.setRead(true);
    serverSelector.onSelect = newclient;
    serverSelector.add(&sockSer);

    clientSelector.setRead(true);
    clientSelector.onSelect = reply;

    clientSelector.start();
    serverSelector.start();

    clientSelector.wait();
    serverSelector.wait();*/

    const unsigned int max_client = 100;
    ntw::srv::Server server(max_client);
    server.start();

    return 0;
};



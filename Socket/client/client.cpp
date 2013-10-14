#include <Socket/SocketSerialized.hpp>
#include <Socket/SelectManager.hpp>

#define NTW_MODE NTW_CLIENT
#include <Socket/FuncWrapper.hpp>

#include <iostream>
#include <chrono>

void broadcast(ntw::SelectManager& selector,ntw::SocketSerialized& sock)
{
}

void reply(ntw::SelectManager& selector,ntw::SocketSerialized& sock)
{
    if (sock.receive() > 0)
    {
        std::cout<<sock<<std::endl;
        /*char* c=0;
        clientSock>>c;
        std::cout<<"[client] recu char*: <"<<c<<">"<<std::endl;
    
        clientSock.clear();
        clientSock<<"message du client";

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        clientSock.send();*/
    }
    else
    {
        std::cerr<<"Server connexion lost"<<std::endl; 
        selector.remove(&sock);
        selector.stop();
    }
};

int main(int argc, char* argv[])
{
    
    //init broadcast sock
    ntw::SocketSerialized* broadcast_sock = new ntw::SocketSerialized(ntw::Socket::Dommaine::IP,ntw::Socket::Type::TCP);
    broadcast_sock->serverMode(NTW_PORT_CLIENT);
    //init broadcast thread
    ntw::SelectManager broadcast_recv;
    broadcast_recv.setRead(true);
    broadcast_recv.onSelect = broadcast;
    broadcast_recv.add(broadcast_sock);
    
    //init request sock
    ntw::SocketSerialized request_sock(ntw::Socket::Dommaine::IP,ntw::Socket::Type::TCP);
    request_sock.connect("127.0.0.1",NTW_PORT_SERVER);
    
    //start listener
    broadcast_recv.start();
    
    /**********************
     ******* CLIENT *******
     *********************/
    if (ntw::FuncWrapper::verifyConnect(request_sock) != NTW_ERROR_NO)
    {
        broadcast_recv.stop();
        broadcast_recv.wait();
        return 1;
    }
    ntw::FuncWrapper::getVersion(request_sock);
    std::cout<<ntw::FuncWrapper::getVersion(request_sock)<<std::endl;


    //wait broadcast closure
    broadcast_recv.stop();
    broadcast_recv.wait();

    return 0;
};



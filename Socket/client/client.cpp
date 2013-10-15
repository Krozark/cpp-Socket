#include <Socket/SocketSerialized.hpp>
#include <Socket/SelectManager.hpp>

#include <Socket/FuncWrapper.hpp>

#include <iostream>
#include <chrono>

void onBroadcastRecv(ntw::SelectManager& broadcast_recv,ntw::SocketSerialized& sock)
{
    std::cout<<"RECV : "<<sock<<std::endl;
    sock.clear();
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
    ntw::SocketSerialized* clientSock = NULL;

    broadcast_sock->serverMode(NTW_PORT_CLIENT);
    //init broadcast thread
    ntw::SelectManager broadcast_recv;
    broadcast_recv.setRead(true);
    broadcast_recv.onSelect = onBroadcastRecv;
    //start listener
    
    //init request sock
    ntw::SocketSerialized request_sock(ntw::Socket::Dommaine::IP,ntw::Socket::Type::TCP);
    request_sock.connect("127.0.0.1",NTW_PORT_SERVER);

    {
        std::cout<<"waiting for server connexion"<<std::endl;
        fd_set readfds;

        timeval timeout;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;//10⁻⁶

        int id = broadcast_sock->id();
        FD_ZERO(&readfds);
        FD_SET(id,&readfds);

        int res = select(id+1,&readfds,NULL,NULL,&timeout);

        if (res<0)
        {
            std::cerr<<"ERROR select()"<<std::endl;
            return 1;
        }
        else if (res == 0)
        {
            std::cerr<<"ERROR select() timeout"<<std::endl;
            return 3;
        }
        if(FD_ISSET(id,&readfds))
        {
            broadcast_recv.start();
            ntw::SocketSerialized* clientSock = new ntw::SocketSerialized(broadcast_sock->accept());
            ntw::FuncWrapper::msg(*clientSock,NTW_WELCOM_MSG,NTW_ERROR_NO);
            broadcast_recv.add(clientSock);
        }
        std::cout<<"Ready"<<std::endl;
    }
    
    /**********************
     ******* CLIENT *******
     *********************/
    std::cout<<"Verify connexion"<<std::endl;
    if (ntw::FuncWrapper::verifyIsConnected(request_sock) != NTW_ERROR_NO)
    {
        return 2;
    }

    std::cout<<ntw::FuncWrapper::getVersion(request_sock)<<std::endl;
    std::cout<<ntw::FuncWrapper::testParamInt(request_sock,25)<<std::endl;

    delete broadcast_sock;
    if(clientSock)
        delete clientSock;
    //wait broadcast closure
    broadcast_recv.stop();
    broadcast_recv.wait();

    return 0;
};



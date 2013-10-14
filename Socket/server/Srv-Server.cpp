#define NTW_MODE NTW_SERVER
#include <Socket/server/Server.hpp>
#include <Socket/FuncWrapper.hpp>

namespace ntw
{
namespace srv
{
    Server::Server(unsigned int max_client,unsigned int min_client,float timeout) :
        new_connexion_sock(ntw::Socket::Dommaine::IP,ntw::Socket::Type::TCP),
        new_connexion_recv(timeout),
        request_recv(true,false,false,onRequestRecv,min_client,max_client,0,timeout),
        broadcast_sender(true,false,false,onBroadCastRecv,min_client,max_client,0,timeout)
    {
        //init sock
        new_connexion_sock.serveurMode();
        //init selector
        new_connexion_recv.setRead(true);
        new_connexion_recv.onSelect = onNewClientRecv;
        //add sock
        new_connexion_recv.add(&new_connexion_sock);
    }

    void Server::start()
    {
        //start
        new_connexion_recv.start();
        request_recv.start();
        broadcast_sender.start();

        //join
        new_connexion_recv.wait();
        request_recv.wait();
        broadcast_sender.wait();
    }

    void Server::stop()
    {
        new_connexion_recv.stop();
        request_recv.stop();
        broadcast_sender.stop();
    }
    void Server::onNewClientRecv(ntw::SelectManager& new_connexion_recv, ntw::SocketSerialized& sock)
    {
        ntw::SocketSerialized* clientSock = new ntw::SocketSerialized(sock.accept());
        Server& self = *((ntw::srv::Server*)((long int)(&new_connexion_recv) - (long int)(&((ntw::srv::Server*)NULL)->new_connexion_recv)));
        bool ok = true;
        if(not (self.request_recv.add(clientSock)))
        {
            ok = false;
            ntw::FuncWrapper::verifyConnect(*clientSock,NTW_ERROR_REQUEST_ADD_MSG,NTW_ERROR_REQUEST_ADD);
        }

        if(ok and not (self.broadcast_sender.add(Socket::Dommaine::IP,Socket::Type::TCP,clientSock->getIp(),clientSock->getPort())))
        {
            std::cout<<"pwet"<<std::endl;
            ok = false;
            self.request_recv.remove(clientSock);
            ntw::FuncWrapper::verifyConnect(*clientSock,NTW_ERROR_DISPATCH_ADD_MSG,NTW_ERROR_DISPATCH_ADD);
        }

        if(not ok)
        {
            clientSock->shutdown();
            delete clientSock;
        }
        else
        {
            ntw::FuncWrapper::verifyConnect(*clientSock,NTW_WELCOM_MSG,NTW_ERROR_NO);
        }            
    }

    void Server::onRequestRecv(ntw::SelectManager& request_recv, ntw::SocketSerialized& sock)
    {
        if(sock.receive() >0)
        {
            ntw::FuncWrapper::dispatch(sock);
        }
        else
        {
            std::cerr<<"Client connection lost <id:"<<sock.id()<<">"<<std::endl; 
            request_recv.remove(&sock);
            delete &sock;
        }
    }

    void Server::onBroadCastRecv(ntw::SelectManager& broadcast_sender, ntw::SocketSerialized& sock)
    {
    }
}
}

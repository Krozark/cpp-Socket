#define NTW_MODE NTW_SERVER
#include <Socket/server/Server.hpp>
#include <Socket/FuncWrapper.hpp>

namespace ntw
{
namespace srv
{
    Server::Server(unsigned int max_client,unsigned int min_client,float timeout) :
        //new_connexion_sock(ntw::Socket::Dommaine::IP,ntw::Socket::Type::TCP),
        //new_connexion_recv(timeout),
        request_recv(true,false,false,onRequestRecv,min_client,max_client,0,timeout)
        //broadcast_sender(true,false,false,onBroadCastRecv,min_client,max_client,0,timeout)
    {
        /*
        //init sock
        new_connexion_sock.serveurMode();
        //init selector
        new_connexion_recv.setRead(true);
        new_connexion_recv.onSelect = onNewClientRecv;
        //add sock
        new_connexion_recv.add(&new_connexion_sock);
        */
    }

    void Server::start()
    {
        //start
        //new_connexion_recv.start();
        request_recv.start();
        //broadcast_sender.start();

        //join
        //new_connexion_recv.wait();
        request_recv.wait();
        //broadcast_sender.wait();
    }

    void Server::stop()
    {
        //new_connexion_recv.stop();
        request_recv.stop();
        //broadcast_sender.stop();
    }
    void Server::onNewClientRecv(ntw::SelectManager& new_connexion_recv, ntw::Socket& sock)
    {
        ntw::SocketSerialized* clientSock = new ntw::SocketSerialized(sock.accept());
        ntw::FuncWrapper::verifyConnect(*clientSock);

        std::cout<<"Recus à "<<clientSock->id()<<std::endl;

        //((ntw::srv::Server*)((long int)(&new_connexion_recv) - (long int)(&((ntw::srv::Server*)NULL)->new_connexion_recv)))->request_recv.add(clientSock);
    }

    void Server::onRequestRecv(ntw::SelectManager& request_recv, ntw::Socket& sock)
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
            request_recv.remove(&clientSock);
            delete &clientSock;
        }
    }

    void Server::onBroadCastRecv(ntw::SelectManager& new_connexion_recv, ntw::Socket& sock)
    {
    }
}
}

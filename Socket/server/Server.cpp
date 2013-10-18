#include <Socket/server/Server.hpp>

#if NTW_MODE == NTW_SERVER
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
        request_recv.setDelete(false);
        broadcast_sender.setDelete(false);
        new_connexion_recv.setDelete(false);
        //init sock
        new_connexion_sock.serverMode(NTW_PORT_SERVER);
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
        Server& self = *((ntw::srv::Server*)((long int)(&new_connexion_recv) - (long int)(&((ntw::srv::Server*)NULL)->new_connexion_recv)));

        self.clients.emplace_back();
        Client& client = self.clients.back();

        sock.accept(client.request_sock);
        bool ok = true;
        if(not (self.request_recv.add(&client.request_sock)))
        {
            ok = false;
            ntw::FuncWrapper::msg(client.request_sock,NTW_ERROR_REQUEST_ADD_MSG,NTW_ERROR_REQUEST_ADD);
        }

        if(ok and not (self.broadcast_sender.add(&client.broadcast_sock,client.request_sock.getIp(),NTW_PORT_CLIENT)))
        {
            ok = false;
            self.request_recv.remove(&client.request_sock);
            ntw::FuncWrapper::msg(client.request_sock,NTW_ERROR_BROADCAST_ADD_MSG,NTW_ERROR_BROADCAST_ADD);
        }

        if(not ok)
        {
            client.request_sock.shutdown();
        }
        else
        {
            ntw::FuncWrapper::msg(client.request_sock,NTW_WELCOM_MSG,NTW_ERROR_NO);
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
            std::cerr<<"[SERVER] onRequest connexion lost <id:"<<sock.id()<<">"<<std::endl; 
            request_recv.remove(&sock);

            /*if(do_delete)
                delete &sock;*/
        }
    }

    void Server::onBroadCastRecv(ntw::SelectManager& broadcast_sender, ntw::SocketSerialized& sock)
    {
        if(sock.receive() >0)
        {
            sock.clear();
            sock<<NTW_ERROR_UNKNOW<<"WTF are you doing right now?";
            sock.sendCl();
        }
        else
        {
            std::cerr<<"[SERVER] onBroadCastRecv connexion lost <id:"<<sock.id()<<">"<<std::endl; 
            broadcast_sender.remove(&sock);

            /*if(do_delete)
                delete &sock;*/
        }
    }
}
}
#endif

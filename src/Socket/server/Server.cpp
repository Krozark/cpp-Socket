#include <Socket/server/Server.hpp>
#include <Socket/define.hpp>

namespace ntw
{
namespace srv
{
    Server::Server(unsigned int port,std::string host,int (*c_dispatch)(int id,SocketSerialized&),unsigned int max_client,unsigned int max_threads,float timeout) :
        on_new_client(nullptr),
        on_delete_client(nullptr),
        new_connexion_sock(ntw::Socket::Domain::IP,ntw::Socket::Type::TCP),
        new_connexion_recv(timeout),
        request_recv(true,false,false,onRequestRecv,this,0,max_client,max_threads,timeout),
        //broadcast_sender(true,false,false,onBroadCastRecv,this,min_client,max_client,0,timeout)
        dispatch(c_dispatch)
    {
        //request_recv.setDelete(false);
        //broadcast_sender.setDelete(false);
        //new_connexion_recv.setDelete(false);
        //init sock
        new_connexion_sock.serverMode(port,max_client,host);
        //init selector
        new_connexion_recv.setRead(true);
        new_connexion_recv.onSelect = onNewClientRecv;
        new_connexion_recv.data = this;
        //add sock
        new_connexion_recv.add(&new_connexion_sock);

    }

    Server::~Server()
    {
        new_connexion_sock.shutdown();
        clients.clear();
    }

    void Server::start()
    {
        //start
        new_connexion_recv.start();
        request_recv.start();

        /*if(Config::broadcast)
            broadcast_sender.start();*/

    }

    void Server::stop()
    {
        new_connexion_recv.stop();
        request_recv.stop();

        /*if(Config::broadcast)
            broadcast_sender.stop();*/

    }

    void Server::wait()
    {
        new_connexion_recv.wait();
        request_recv.wait();

        /*if(Config::broadcast)
            broadcast_sender.wait();*/
    }
    void Server::onNewClientRecv(ntw::SelectManager& new_connexion_recv,void* data, ntw::SocketSerialized& sock)
    {
        Server& self = *(ntw::srv::Server*)data;

        self.client_mutex.lock();
        self.clients.emplace_back();
        Client& client = self.clients.back();
        self.client_mutex.unlock();

        sock.accept(client.request_sock);
        bool ok = true;
        if(not (self.request_recv.add(&client.request_sock)))
        {
            ok = false;
            ntw::FuncWrapper::msg(client.request_sock,NTW_ERROR_REQUEST_ADD_MSG,Status::request_add);
        }

        /*if(Config::broadcast)
        {
            if(ok and not (self.broadcast_sender.add(&client.broadcast_sock,client.request_sock.getIp(),Config::port_client)))
            {
                ok = false;
                self.request_recv.remove(&client.request_sock);
                ntw::FuncWrapper::msg(client.request_sock,NTW_ERROR_BROADCAST_ADD_MSG,NTW_ERROR_BROADCAST_ADD);
            }
        }*/

        if(not ok)
        {
            client.request_sock.shutdown();
        }
        else
        {
            if(self.on_new_client)
                self.on_new_client(self,client);
            ntw::FuncWrapper::msg(client.request_sock,NTW_WELCOM_MSG,Status::ok);
        }
    }

    void Server::onRequestRecv(ntw::SelectManager& request_recv,void* data, ntw::SocketSerialized& sock)
    {
        bool rm = true;
        Server& self = *(Server*)data;
        if(sock.receive() >0)
        {
            try
            {
                ntw::FuncWrapper::srv::dispatch(sock,self.dispatch);
                rm = false;
            }
            catch (ntw::SocketExeption& e)
            {
                //rm = true;
            }
        }

        if(rm)
        {
            std::cerr<<"[SERVER] onRequest connexion lost <id:"<<sock.id()<<">"<<std::endl;
            Client* client = ((ntw::srv::Client*)((long long int)(&sock) - (long long int)(&((ntw::srv::Client*)NULL)->request_sock)));

            self.remove (client);
        }
    }

    /*void Server::onBroadCastRecv(ntw::SelectManager& broadcast_sender, void* data,ntw::SocketSerialized& sock)
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


            Server& self = *(ntw::srv::Server*)data;
            Client* client = ((ntw::srv::Client*)((long int)(&sock) - (long int)(&((ntw::srv::Client*)NULL)->broadcast_sock)));

            self.remove(client);
        }
    }*/

    bool Server::remove(Client* client_ptr)
    {
        if(this->on_delete_client)
            this->on_delete_client(*this,*client_ptr);

        client_mutex.lock();
        auto begin = clients.begin();
        bool res = false;
        while(begin != clients.end())
        {
            if(&(*begin) == client_ptr)
            {
                res = true;
                Client& client = *client_ptr;

                request_recv.remove(&client.request_sock);
                client.request_sock.shutdown();

                /*if(Config::broadcast)
                {
                    broadcast_sender.remove(&client.broadcast_sock);
                    client.broadcast_sock.shutdown();
                }*/

                begin = clients.erase(begin);
            }
            else
                ++begin;
        }
        client_mutex.unlock();
        return res;
    }
}
}

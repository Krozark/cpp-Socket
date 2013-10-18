#include <Socket/client/Client.hpp>
#include <Socket/FuncWrapper.hpp>

namespace ntw
{
    namespace cli
    {
        Client::Client() :  request_sock(Socket::Dommaine::IP,Socket::Type::TCP),
        new_broadcast_sock(Socket::Dommaine::IP,Socket::Type::TCP),
        broadcast_recv_sock(Socket::Dommaine::IP,Socket::Type::TCP)
        {
            //init broadcast
            new_broadcast_sock.serverMode(NTW_PORT_CLIENT);

            broadcast_recv.setRead(true);
            broadcast_recv.setDelete(false);
            broadcast_recv.onSelect = onBroadcastRecv;
            broadcast_recv.data = this;
        }

        Client::~Client()
        {
            request_sock.shutdown();
            new_broadcast_sock.shutdown();
            broadcast_recv_sock.shutdown();
        }

        int Client::connect(const std::string& host,int port)
        {   
            //init request sock
            request_sock.connect(host,port);
            std::cout<<"waiting for server connexion"<<std::endl;
            fd_set readfds;

            timeval timeout;
            timeout.tv_sec = 5;
            timeout.tv_usec = 0;//10⁻⁶

            int id = new_broadcast_sock.id();
            FD_ZERO(&readfds);
            FD_SET(id,&readfds);

            int res = select(id+1,&readfds,NULL,NULL,&timeout);

            if (res<0)
            {
                std::cerr<<"ERROR select()"<<std::endl;
                return NTW_ERROR_SELECT;
            }
            else if (res == 0)
            {
                std::cerr<<"ERROR select() timeout"<<std::endl;
                return NTW_ERROR_TIMEOUT;
            }
            if(FD_ISSET(id,&readfds))
            {
                broadcast_recv.start();
                new_broadcast_sock.accept(broadcast_recv_sock);
                ntw::FuncWrapper::msg(broadcast_recv_sock,NTW_WELCOM_MSG,NTW_ERROR_NO);
                broadcast_recv.add(&broadcast_recv_sock);
            }
            else
            {
                return NTW_ERROR_CONNEXION;
            }
            std::cout<<"Verify connexion"<<std::endl;
            if (ntw::FuncWrapper::verifyIsConnected(request_sock) != NTW_ERROR_NO)
            {
                return NTW_ERROR_CONNEXION;
            }
            std::cout<<"Ready"<<std::endl;
            return NTW_ERROR_NO;
        }

        void Client::stop()
        {
            broadcast_recv.stop();
        }

        void Client::wait()
        {
            broadcast_recv.wait();
        }

        void Client::onBroadcastRecv(SelectManager& broadcast_recv,void* data,SocketSerialized& sock)
        {
            if(sock.receive() >0)
            {
                std::cout<<"RECV : "<<sock<<std::endl;
                sock.clear();
            }
            else
            {
                std::cerr<<"[SERVER] onBroadcastRecv connexion lost <id:"<<sock.id()<<">"<<std::endl;
            }
        }
    }
}

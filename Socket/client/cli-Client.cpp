#include <Socket/client/Client.hpp>
#include <Socket/FuncWrapper.hpp>

namespace ntw
{
    namespace cli
    {
        Client::Client() : new_broadcast_sock(0), broadcast_recv_sock(0), request_sock(Socket::Dommaine::IP,Socket::Type::TCP)
        {
            //init broadcast
            new_broadcast_sock = new SocketSerialized(Socket::Dommaine::IP,Socket::Type::TCP);
            new_broadcast_sock->serverMode(NTW_PORT_CLIENT);

            broadcast_recv.setRead(true);
            broadcast_recv.onSelect = onBroadcastRecv;
        }

        Client::~Client()
        {
            if(new_broadcast_sock)
                delete new_broadcast_sock;
            /*if(broadcast_recv_sock)
                delete broadcast_recv_sock;*/
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

            int id = new_broadcast_sock->id();
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
                broadcast_recv_sock = new ntw::SocketSerialized(new_broadcast_sock->accept());
                ntw::FuncWrapper::msg(*broadcast_recv_sock,NTW_WELCOM_MSG,NTW_ERROR_NO);
                broadcast_recv.add(broadcast_recv_sock);
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

        void Client::onBroadcastRecv(SelectManager& broadcast_recv,SocketSerialized& sock)
        {
            std::cout<<"RECV : "<<sock<<std::endl;
            sock.clear();
        }
    }
}
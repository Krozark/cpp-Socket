#ifndef NTW_SRV_SERVEUR_HPP
#define NTW_SRV_SERVEUR_HPP


#include <Socket/SocketSerialized.hpp>
#include <Socket/BalancingSelector.hpp>

namespace ntw
{
namespace srv
{
    class Server
    {
        public:
            Server(unsigned int max_client,unsigned int min_client=1,float timeout=NTW_DEFAULT_TIMEOUT);
            Server(const Server&) = delete;
            Server& operator=(const Server&) = delete;

            void start();
            void stop();

        private:
            ntw::SocketSerialized* new_connexion_sock;
            ntw::SelectManager new_connexion_recv;
            ntw::BalancingSelector request_recv;
            ntw::BalancingSelector broadcast_sender;

            static void onNewClientRecv(ntw::SelectManager& new_connexion_recv, ntw::SocketSerialized& sock);
            static void onRequestRecv(ntw::SelectManager& new_connexion_recv, ntw::SocketSerialized& sock);
            static void onBroadCastRecv(ntw::SelectManager& new_connexion_recv, ntw::SocketSerialized& sock);
    };
}
}

#endif

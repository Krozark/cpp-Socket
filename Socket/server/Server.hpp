#ifndef NTW_SRV_SERVEUR_HPP
#define NTW_SRV_SERVEUR_HPP


#include <Socket/SocketSerialized.hpp>
#include <Socket/BalancingSelector.hpp>
#include <Socket/server/Client.hpp>

#include <list>

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

                ~Server();

                void start();
                void stop();
                void wait();

            protected:
                ntw::SocketSerialized new_connexion_sock;
                ntw::SelectManager new_connexion_recv;
                ntw::BalancingSelector request_recv;
                ntw::BalancingSelector broadcast_sender;

                static void onNewClientRecv(ntw::SelectManager& new_connexion_recv,void* data, ntw::SocketSerialized& sock);
                static void onRequestRecv(ntw::SelectManager& new_connexion_recv,void* data, ntw::SocketSerialized& sock);
                static void onBroadCastRecv(ntw::SelectManager& new_connexion_recv, void* data,ntw::SocketSerialized& sock);

                bool remove(Client* client);

                std::list<Client> clients;
                std::mutex client_mutex;

        };
    }
}

#endif

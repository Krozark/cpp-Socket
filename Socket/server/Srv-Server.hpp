#ifndef NTW_SRV_SERVEUR_HPP
#define NTW_SRV_SERVEUR_HPP

#include <Socket/BalancingSelector.hpp>

namespace ntw
{
namespace srv
{
    class Serveur
    {
        public:
            Serveur(unsigned int max_client,unsigned int min_client=1,float timeout=NTW_DEFAULT_TIMEOUT);
            Serveur(const Serveur&) = delete;
            Serveur& operator=(const Serveur&) = delete;

            void start();
            void stop();

        private:
            ntw::SelectManager new_connexion_recv;
            ntw::BalancingSelector request_recv;
            ntw::BalancingSelector broadcast_sender;

            static void onNewClientRecv(ntw::SelectManager& new_connexion_recv, ntw::Socket& sock);
            static void onRequestRecv(ntw::SelectManager& new_connexion_recv, ntw::Socket& sock);
            static void onBroadCastRecv(ntw::SelectManager& new_connexion_recv, ntw::Socket& sock);
    };
}
}

#endif

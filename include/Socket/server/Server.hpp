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
        /**
         * \brief A class to make a TCP server
         */
        class Server
        {
            public:
                /**
                 * \brief constructor
                 * \param max_client the number of max client
                 * \param min_client the minimal client number
                 * \param timeout the timeout for the select operation
                 */
                Server(unsigned int max_client,unsigned int min_client=1,float timeout=Config::default_timeout);
                Server(const Server&) = delete;
                Server& operator=(const Server&) = delete;

                /**
                 * \brief destructor
                 */
                ~Server();

                /**
                 * \brief Start the server
                 */
                void start();

                /**
                 * \brief Stop the server
                 */
                void stop();

                /**
                 * \brief wait for the end of internal threads
                 */
                void wait();

            protected:
                ntw::SocketSerialized new_connexion_sock; ///< socket tha manage new connextion
                ntw::SelectManager new_connexion_recv; ///< manager for nex connextion

                ntw::BalancingSelector request_recv; ///< socket for broadcast new connection
                ntw::BalancingSelector broadcast_sender; ///< manager for broadcast

                /**
                 * \brief callback for new client
                 */
                static void onNewClientRecv(ntw::SelectManager& new_connexion_recv,void* data, ntw::SocketSerialized& sock);

                /**
                 * \brief callback of a request of a connected client
                 */
                static void onRequestRecv(ntw::SelectManager& new_connexion_recv,void* data, ntw::SocketSerialized& sock);

                /**
                 * \brief callback on datat receive on broadcast
                 */
                static void onBroadCastRecv(ntw::SelectManager& new_connexion_recv, void* data,ntw::SocketSerialized& sock);

                /**
                 * \brief remove a client
                 */
                bool remove(Client* client);

                std::list<Client> clients; ///< internal clients
                std::mutex client_mutex; ///< client mutex
        };
    }
}

#endif

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
                 * \param port the server port
                 * \param host the host to use. set it empty for default INADDR_ANY
                 * \param dispatch the dispatch callback function
                 * \param max_client the number of max client per thread
                 * \param max_threads the number of threads, 0 is infinite
                 * \param timeout the timeout for the select operation
                 */
                Server(unsigned int port,std::string host,int (*dispatch)(int id,SocketSerialized&),unsigned int max_client,unsigned int max_threads=0,float timeout=5.f);
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

                void (*on_new_client)(ntw::srv::Server& self,ntw::srv::Client& client);
                void (*on_delete_client)(ntw::srv::Server& self,ntw::srv::Client& client);

                int port()const;

            protected:
                ntw::SocketSerialized new_connexion_sock; ///< socket tha manage new connextion
                ntw::SelectManager new_connexion_recv; ///< manager for nex connextion
                ntw::BalancingSelector request_recv; ///< socket managaer for request (TCP)

                /**
                 * \brief callback for new client
                 */
                static void onNewClientRecv(ntw::SelectManager& new_connexion_recv,void* data, ntw::SocketSerialized& sock);

                /**
                 * \brief callback of a request of a connected client
                 */
                static void onRequestRecv(ntw::SelectManager& new_connexion_recv,void* data, ntw::SocketSerialized& sock);

                /**
                 * \brief remove a client
                 */
                bool remove(Client* client);

                std::list<Client> clients; ///< internal clients
                std::mutex client_mutex; ///< client mutex

                int (*dispatch)(int id,SocketSerialized& request);

                const int _port;
        };
    }
}

#endif

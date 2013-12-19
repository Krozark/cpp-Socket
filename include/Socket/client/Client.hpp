#ifndef NTW_CLIENT_HPP
#define NTW_CLIENT_HPP

#include <Socket/SocketSerialized.hpp>
#include <Socket/SelectManager.hpp>
#include <Socket/Config.hpp>
#include <string>

namespace ntw
{
    class SelectManager;
    class SocketSerialized;

    /**
     * \brief Client namespace
     */
    namespace cli
    {
        /**
         * \brief A TCP client
         */
        class Client
        {
            public:
                /**
                 * \brief Constructor
                 */
                Client();
                Client(const Client&) = delete;
                Client& operator=(const Client&) = delete;

                /**
                 * \brief Destructor
                 */
                ~Client();
                
                /**
                 * \brief connect the client to the server
                 * \param host the host of the server
                 * \param port the server port to connect to
                 * \return NTW_ERROR_NO if ok
                 */
                int connect(const std::string& host,int port=Config::port_server);
                /**
                 * \brief stop the broadcast reciver
                 */
                void stop();

                /**
                 * \broadcast waite for the end of the broadcast reciver
                 */
                void wait();

                /**
                 * \brief Call a function on the server.
                 * Note: function have to be  created using:
                 *
                 * "return ntw::FuncWrapper::cli::send(SocketSerialized& sock,int id,Args&& ... args);"
                 *
                 * \param pf the function to call
                 * \param args the function param
                 * \return the retun function value
                 */
                template<typename Ret,typename ... Args>
                Ret call(Ret (*pf)(SocketSerialized&, Args ...),Args&& ... args);

                SocketSerialized request_sock; ///< internal socket for communication
            private:

                SocketSerialized new_broadcast_sock; ///< internal socket for broadcast listener
                SocketSerialized broadcast_recv_sock; ///< internal Soket for broadcast reciver
                SelectManager broadcast_recv; ///< manage the broadcast
                static void onBroadcastRecv(SelectManager& broadcast_recv,void* data,SocketSerialized& sock); ///< callback of the broadcast
        };
    }
}
#include <Socket/client/Client.tpl>
#endif

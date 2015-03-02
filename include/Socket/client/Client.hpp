#ifndef NTW_CLIENT_HPP
#define NTW_CLIENT_HPP

#include <Socket/SocketSerialized.hpp>
#include <Socket/SelectManager.hpp>
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
                int connect(const std::string& host,int port);

                /***
                 * \brief close all open socket
                 */
                void disconnect();

                /**
                 * \brief stop the broadcast reciver
                 */
                //void stop();

                /**
                 * \broadcast waite for the end of the broadcast reciver
                 */
                //void wait();

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

                /**
                 * \brief Call a function on the server of id=id.
                 * "return ntw::FuncWrapper::cli::send(SocketSerialized& sock,int id,Args&& ... args);"
                 * Ret have to be define as template parameter
                 * \param id the functiuon id to call
                 * \param args the function param
                 * \return the retun function value
                 */
                template<typename Ret,typename ... Args>
                Ret call(int id,Args&& ... args);

                SocketSerialized request_sock; ///< internal socket for communication
            private:

        };
    }
}
#include <Socket/client/Client.tpl>
#endif

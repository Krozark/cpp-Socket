#ifndef NTW_SRV_CLIENT
#define NTW_SRV_CLIENT

#include <string>
#include <Socket/SocketSerialized.hpp>

namespace ntw
{
    /**
     * \brief namespace for server app
     */
    namespace srv
    {
        class Server;

        /**
         * \brief a server side client representation
         */
        class Client
        {
            public:
                /**
                 * \brief constructor
                 */
                explicit Client();
                Client(const Client&) = delete;
                Client& operator=(const Client&) = delete;
                
                /**
                 * \brief Compare 2 clients
                 */
                bool operator==(const Client& other);

                const SocketSerialized& sock()const;
            private:
                friend class Server;

                SocketSerialized request_sock; ///< the request to communicate
                SocketSerialized broadcast_sock; ///< the request to send broadcast data
        };
    }
}

#endif

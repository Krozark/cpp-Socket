#ifndef NTW_SRV_CLIENT
#define NTW_SRV_CLIENT

#include <string>
#include <Socket/SocketSerialized.hpp>

namespace ntw
{

    namespace srv
    {
        class Server;

        class Client
        {
            public:
                explicit Client();
                Client(const Client&) = delete;
                Client& operator=(const Client&) = delete;
                
                //bool isAnonymous();
                //bool isLogged();

                bool operator==(const Client& other);
            private:
                friend class Server;

                SocketSerialized request_sock;
                SocketSerialized broadcast_sock;
        };
    }
}

#endif

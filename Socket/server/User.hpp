#ifndef NTW_USER
#define NTW_USER

#include <string>
#include <Socket/SocketSerialized.hpp>

namespace ntw
{

    namespace srv
    {
        class Server;

        class User
        {
            public:
                explicit User();
                User(const User&) = delete;
                User& operator=(const User&) = delete;
                
                bool isAnonymous();
                bool isLogged();
            private:
                friend class Server;

                int pk;
                SocketSerialized request_sock;
                SocketSerialized broadcast_sock;
        };
    }
}

#endif

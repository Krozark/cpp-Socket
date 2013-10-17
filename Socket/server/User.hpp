#ifndef NTW_USER
#define NTW_USER

#include <string>
#include <Socket/SocketSerialized.hpp>

namespace ntw
{

    class User
    {
        public:
            explicit User();
            User(const User&) = delete;
            User& operator=(const User&) = delete;
            
            bool isAnonymous();
            bool isLogged();
        private:
            int pk;
            union{SocketSerialized sock_recv;};
            union{SocketSerialized sock_broadcast;};

    };
}

#endif

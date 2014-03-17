#include <Socket/server/Client.hpp>
#include <algorithm>

namespace ntw
{
    namespace srv
    {
        Client::Client() : request_sock(ntw::Socket::Dommaine::IP,ntw::Socket::Type::TCP),
            broadcast_sock(ntw::Socket::Dommaine::IP,ntw::Socket::Type::TCP)
        {
            std::cout<<"[CONSTRUCTOR] srv::Client <"<<this<<">"<<std::endl;
        }

        Client::~Client()
        {
            std::cout<<"[DESTRUCTOR] srv::Client <"<<this<<">"<<std::endl;
        }

        bool Client::operator==(const Client& other)
        {
            return this == &other;
        }

        const SocketSerialized& Client::sock()const
        {
            return request_sock;
        }

        /// PRIVATE
    }

}

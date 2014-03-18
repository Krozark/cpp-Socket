#include <Socket/server/Client.hpp>
#include <algorithm>

namespace ntw
{
    namespace srv
    {
        Client::Client() : request_sock(ntw::Socket::Dommaine::IP,ntw::Socket::Type::TCP),
            broadcast_sock(ntw::Socket::Dommaine::IP,ntw::Socket::Type::TCP)
        {
        }

        Client::~Client()
        {
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

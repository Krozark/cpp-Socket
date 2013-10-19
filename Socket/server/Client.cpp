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

        bool Client::operator==(const Client& other)
        {
            return this == &other;
        }

        /// PRIVATE
    }

}

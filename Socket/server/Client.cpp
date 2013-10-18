#include <Socket/server/Client.hpp>
#include <algorithm>

namespace ntw
{
    namespace srv
    {
        Client::Client() : request_sock(ntw::Socket::Dommaine::IP,ntw::Socket::Type::TCP),
            broadcast_sock(ntw::Socket::Dommaine::IP,ntw::Socket::Type::TCP)
        {
            //pk = 42; ///<\todo TODO
        }

        /*bool Client::login(std::string username, std::string pass) 
        {
            return true;
        }*/
        /*bool operator==(const Client& other)
        {
        }

        bool Client::isAnonymous()
        {
            return pk <=0;
        }

        bool Client::isLogged()
        {
            return pk >0;
        }*/

        /// PRIVATE
    }

}

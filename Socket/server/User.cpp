#include <Socket/server/User.hpp>
#include <algorithm>

namespace ntw
{
    namespace srv
    {
        User::User() : request_sock(ntw::Socket::Dommaine::IP,ntw::Socket::Type::TCP),
            broadcast_sock(ntw::Socket::Dommaine::IP,ntw::Socket::Type::TCP)
        {
            pk = 42; ///<\todo TODO
        }

        /*bool User::login(std::string username, std::string pass) 
        {
            return true;
        }*/

        bool User::isAnonymous()
        {
            return pk <=0;
        }

        bool User::isLogged()
        {
            return pk >0;
        }

        /// PRIVATE
    }

}

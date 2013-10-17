#include <Socket/server/User.hpp>
#include <algorithm>

namespace ntw
{
    User::User() /*: sock_recv(0), sock_broadcast(0)*/
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

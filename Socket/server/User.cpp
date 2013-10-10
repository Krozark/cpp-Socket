#include <Socket/server/User.hpp>

namespace ntw
{
    User::User(User&& tmp)
    {
        pk = tmp.pk;
    }

    User::login(std::string username, std::string pass)
    {
        return User(username,pass);
    }

    bool User::isAnonymous()
    {
        return pk <=0;
    }

    bool User::isLogged()
    {
        return pk >0;
    }

    /// PRIVATE

    User::User(std::string username, std::string pass)
    {
        pk = 42; ///<\todo TODO
    }
}

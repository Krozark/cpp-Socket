#ifndef NTW_USER
#define NTW_USER

#include <string>

namespace ntw
{
    class User
    {
        public:
            User(User&& tmp);
            User(const User&) = delete;
            User& operator=(const User&) = delete;
            
            static User login(std::string username,std::string pass);

            bool isAnonymous();
            bool isLogged();
        private:
            int pk;

            explicit User(std::string username,std::string pass);
    };
}

#endif

#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <Socket/Config.hpp>
#include <Socket/SocketSerialized.hpp>

/**
 * \brief Define the functions ids
 */
enum FUNCTION_ID
{
    RECV_MSG=1,
};

/**
 * \brief Define the ERRORS values
 */
enum ERRORS {
    INPUT_NOT_VALID = 1,
    EMPTY_INPUT = 2,
};


/**
 * \brief define some external class
 */
namespace ntw {
    namespace srv {
        class Server;
        class Client;
    }
}

void recv_msg(ntw::SocketSerialized& sock,std::string msg);

/**
 * \brief call back on new client recv
 */
void register_client(ntw::srv::Server& self,ntw::srv::Client& client);

/**
 * \brief call back on delete client
 */
void unregister_client(ntw::srv::Server& self,ntw::srv::Client& client);

#endif

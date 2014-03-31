#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

/**
 * \brief define some class used later
 */
namespace ntw
{
    namespace cli
    {
        class Client;
    }
}

/**
 * \brief define some class used later
 */
namespace harpe
{
    class Sequence;
}

#define RECV_MSG 1 ///< the function id in the server
/**
 * \brief start the client
 * \param client the client to start
 */
void run(ntw::cli::Client& client);

#endif

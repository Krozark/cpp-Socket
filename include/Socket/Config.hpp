#ifndef NTW_CONFIG_HPP
#define NTW_CONFIG_HPP

namespace ntw
{
    /**
     * \brief A class that contains default configurations
     */
    class Config
    {
        public:
            Config() = delete;
            Config(const Config&) = delete;
            Config& operator=(const Config&) = delete;

            static int max_connexion; ///< number of clients connected
            static int port_server; ///< port of the Server
            static int port_client; ///< port of the Client
            static double default_timeout; ///< default timeout for SelectManager

    };
}
#endif

#include <Socket/Config.hpp>

namespace ntw
{
    int Config::max_connexion = 5;
    int Config::port_server = 3987;
    int Config::port_client = 3988;
    double Config::default_timeout = 5.f;
    bool Config::broadcast = false;
}

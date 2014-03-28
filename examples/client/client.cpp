#include "defines.hpp"
#include "functions.hpp"
///data base
#include <Socket/Config.hpp>
#include <Socket/client/Client.hpp>


#define SERVER_HOST 1
#define SERVER_PORT 2

int main(int argc,char* argv[])
{
    if(argc < SERVER_PORT+1)
    {
        std::cout<<"Usage are: "<<argv[0]<<" <server-host> <server-port>"<<std::endl;
        return 1;
    }
    
    //// inti config
    ntw::Config::default_timeout = 5.f;
    ntw::Config::broadcast = false;
    ntw::Config::port_server = atoi(argv[SERVER_PORT]);

    ntw::cli::Client client;
    client.connect(argv[SERVER_HOST],ntw::Config::port_server);

    run(client);

    return 0;
}

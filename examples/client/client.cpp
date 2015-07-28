#include "defines.hpp"
#include "functions.hpp"
///data base
#include <Socket/client/Client.hpp>
#include <Socket/Status.hpp>


#define SERVER_HOST 1
#define SERVER_PORT 2

int main(int argc,char* argv[])
{
    if(argc < SERVER_PORT+1)
    {
        std::cout<<"Usage are: "<<argv[0]<<" <server-host> <server-port>"<<std::endl;
        return 1;
    }


    ntw::cli::Client client;
    if(client.connect(argv[SERVER_HOST],atoi(argv[SERVER_PORT])) != ntw::Status::connexion)
        run(client);

    return 0;
}

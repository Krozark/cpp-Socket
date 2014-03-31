#include <Socket/server/Server.hpp>
#include <Socket/Config.hpp>

#include <csignal>
#include <iostream>

#include "functions.hpp"


ntw::srv::Server* server = nullptr;

void stop_server_handler(int sig)
{
    std::cout<<"Recv signal "<<sig<<". Stoping server.\n Please wait."<<std::endl;
    if(server)
        server->stop();
}

#define SERVER_PORT 1


int main(int argc,char* argv[])
{
        if(argc < SERVER_PORT +1)
    {
        std::cout<<"Usage are: "<<argv[0]<<"[server-port]"<<std::endl;
        return 1;
    }

    ntw::Config::port_server = atoi(argv[SERVER_PORT]);
    //ntw::Config::port_client = 3988;
    ntw::Config::max_connexion = 10;
    ntw::Config::default_timeout = 5.f;
    ntw::Config::broadcast = false;
    const unsigned int max_client = 100;


    std::cout<<"[Server start] on:"
        <<"\n\tPort : "<<ntw::Config::port_server
        //<<"\n\tclient port : "<<ntw::Config::port_client
        <<std::endl;

    std::signal(SIGINT, stop_server_handler);

    try
    {
        ntw::Socket::init();
        server = new ntw::srv::Server(max_client);
        server->on_new_client = register_client;
        server->on_delete_client = unregister_client;

        server->start();
        server->wait();
        delete server;
        ntw::Socket::close();
    }
    catch(ntw::SocketExeption& e)
    {
        std::cout<<e.what()<<std::endl;
    }

    std::cout<<"Server is close"<<std::endl;
    std::cout<<"Good bye"<<std::endl;

    return 0;
}





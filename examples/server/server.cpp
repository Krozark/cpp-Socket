#include <Socket/server/Server.hpp>

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

    const unsigned int max_client = 100;


    std::cout<<"[Server start] on:"
        <<"\n\tPort : "<<argv[SERVER_PORT]
        <<std::endl;

    std::signal(SIGINT, stop_server_handler);

    try
    {
        server = new ntw::srv::Server(atoi(argv[SERVER_PORT]),"127.0.0.1",dispatch,max_client);
        server->on_new_client = register_client;
        server->on_delete_client = unregister_client;

        server->start();
        server->wait();
        delete server;
    }
    catch(ntw::SocketExeption& e)
    {
        std::cout<<e.what()<<std::endl;
    }

    std::cout<<"Server is close"<<std::endl;
    std::cout<<"Good bye"<<std::endl;

    return 0;
}





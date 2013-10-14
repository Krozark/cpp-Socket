#include <Socket/server/Server.hpp>

#include <iostream>
#include <chrono>

int main(int argc, char* argv[])
{

    const unsigned int max_client = 100;
    ntw::srv::Server server(max_client);
    server.start();

    return 0;
};



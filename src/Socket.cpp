#include <utility>
#include <iostream>

#include "Socket.hpp"

namespace ntw {

int Socket::Max_clients = 5;
int Socket::Buffer_size = 1024;

Socket::Socket(Socket::Dommaine dommaine,Socket::Type type,int protocole)
{
    //déclaration de la socket
    if((sock = socket(dommaine,type,protocole)) == INVALID_SOCKET)
    {
        perror("socket()");
        throw "Invalid socket";
    }

    //sin_family = Dommaine
    sock_cfg.sin_family = dommaine;
};

Socket::~Socket()
{
    _close();
};

void Socket::Connect(std::string host,int port)
{
    if(host.size() == 0)
        sock_cfg.sin_addr.s_addr = htonl(INADDR_ANY); //IP automatiquement chopée utiliser inet_addr("127.0.0.1") pour présiser
    else
        sock_cfg.sin_addr.s_addr= inet_addr(host.c_str());

    //sin_port = port à utiliser
    sock_cfg.sin_port = htons(port);

     if(connect(sock, (SOCKADDR*)&sock_cfg, sizeof(sock_cfg)) != SOCKET_ERROR)
            std::cout<<"Connexion à "<<inet_ntoa(sock_cfg.sin_addr)<<" sur le port "<<htons(sock_cfg.sin_port)<<std::endl;
        else
            std::cout<<"Impossible de se connecter"<<std::endl;;
};

Socket Socket::Wait(std::string host,int port)
{
    //sin_addr.s_addr = adresse IP à utiliser
    //IP automatiquement chopée
    if(host.size() == 0)
        sock_cfg.sin_addr.s_addr = htonl(INADDR_ANY);
    else
        sock_cfg.sin_addr.s_addr= inet_addr(host.c_str());

    //sin_port = port à utiliser
    sock_cfg.sin_port = htons(port);

    if(bind(sock,(SOCKADDR*)&sock_cfg,sizeof(sock_cfg)) == SOCKET_ERROR)
    {
        perror("bind()");
        throw "Ennable to bind soket";
    }

    if(listen(sock,Socket::Max_clients) == SOCKET_ERROR)
    {
        perror("listen()");
        throw "Ennable to listen";
    }

    socklen_t size = sizeof(SOCKADDR_IN);
    Socket client;
    std::cout<<"Patientez pendant que le client se connecte sur le port "<<htons(sock_cfg.sin_port)<<std::endl;
    client.sock = accept(sock,(SOCKADDR*) &client.sock_cfg, &size);
    std::cout<<"Un client se connecte avec la socket "<<client.sock<<" de "<<inet_ntoa(client.sock_cfg.sin_addr)<<":"<<htons(client.sock_cfg.sin_port)<<std::endl;
    return client;
};

void Socket::Shutdown(Socket::Down mode)
{
    if (shutdown(sock,mode) == -1)
    {
        perror("Shutdown()");
        throw "Can't shutdown socket";
    }
};

};


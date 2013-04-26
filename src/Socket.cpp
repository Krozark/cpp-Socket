#include <utility>
#include <iostream>

#include "Socket.hpp"
#include <string.h>

namespace ntw {
int Socket::max_id = 0;

Socket::Socket(Socket::Dommaine dommaine,Socket::Type type,int protocole) : sock(INVALID_SOCKET)
{
    //déclaration de la socket
    if((sock = socket(dommaine,type,protocole)) == INVALID_SOCKET)
    {
        perror("socket()");
        throw SocketExeption("Invalid socket");
    }

     max_id = (sock>max_id)?sock:max_id;

    //sin_family = Dommaine
    sock_cfg.sin_family = dommaine;
    memset(&(sock_cfg.sin_zero),'\0',8); // mise a 0
};

Socket::Socket() : sock(INVALID_SOCKET)
{
}

Socket::~Socket()
{
    _close();
};

void Socket::Connect(std::string host,int port)
{
    //sin_addr.s_addr =  adresse IP
    sock_cfg.sin_addr.s_addr = inet_addr(host.c_str());
    //sin_port = port à utiliser
    sock_cfg.sin_port = htons(port);

    if(connect(sock, (SOCKADDR*)&sock_cfg, sizeof(sockaddr)) != SOCKET_ERROR)
        std::cerr<<"<id:"<<sock<<">Connect to "<<inet_ntoa(sock_cfg.sin_addr)<<":"<<htons(sock_cfg.sin_port)<<std::endl;
    else
        std::cerr<<"<id:"<<sock<<">Ennable to connect"<<std::endl;;
};

void Socket::Bind()
{
    if(bind(sock,(SOCKADDR*)&sock_cfg,sizeof(sock_cfg)) == SOCKET_ERROR)
    {
        perror("bind()");
        throw SocketExeption("Ennable to bind soket");
    }
}

void Socket::Listen(const int max_connexion)
{
    if(listen(sock,max_connexion) == SOCKET_ERROR)
    {
        perror("listen()");
        throw SocketExeption("Ennable to listen");
    }
}

void Socket::ServeurMode(const int max_connexion,std::string host,int port)
{
    //sin_addr.s_addr = adresse IP à utiliser
    //IP automatiquement chopée
    if(host.size() == 0)
        sock_cfg.sin_addr.s_addr = htonl(INADDR_ANY);
    else
        sock_cfg.sin_addr.s_addr= inet_addr(host.c_str());

    //sin_port = port à utiliser
    sock_cfg.sin_port = htons(port);

    Bind();
    Listen(max_connexion);

};

Socket Socket::Accept()
{
    Socket client;
    Accept(client);
    return client;
};

void Socket::Accept(Socket& client)
{
    socklen_t size = sizeof(sockaddr_in);
    std::cerr<<"<id:"<<sock<<">Waiting a new connection to "<<inet_ntoa(sock_cfg.sin_addr)<<":"<<htons(sock_cfg.sin_port)<<std::endl;
    client.sock = accept(sock,(sockaddr*) &(client.sock_cfg), &size);
    if (client.sock == INVALID_SOCKET)
    {
        perror("accept()");
        throw SocketExeption("Invalid socket get");
    }
    std::cerr<<"<id:"<<sock<<">New connection accept <id:"<<client.sock<<"> from "<<inet_ntoa(client.sock_cfg.sin_addr)<<":"<<htons(client.sock_cfg.sin_port)<<std::endl;
};

void Socket::Shutdown(Socket::Down mode)
{
    if (shutdown(sock,mode) == -1)
    {
        perror("shutdown()");
        throw SocketExeption("Can't shutdown socket");
    }
};

};


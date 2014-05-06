#include <utility>
#include <iostream>

#include <Socket/Socket.hpp>
#include <Socket/FuncWrapper.hpp>

#include <string.h>


namespace ntw {
//int Socket::max_id = 0;

#if __WIN32
WSADATA Socket::WSAData;
#endif

Socket::Socket(Socket::Dommaine dommaine,Socket::Type type,int protocole) : sock(INVALID_SOCKET), need_connect(type == Socket::Type::TCP)
{
    //déclaration de la socket
    if((sock = ::socket(dommaine,type,protocole)) == INVALID_SOCKET)
    {
        ::perror("socket()");
        throw SocketExeption("Invalid socket");
    }

    ::memset((char*)&sock_cfg,0,sizeof(sock_cfg)); // mise a 0
    sock_cfg.sin_family = dommaine;

};

Socket::Socket(bool need_conn) : sock(INVALID_SOCKET), need_connect(need_conn)
{
}

Socket::~Socket()
{
    _close();
};

bool Socket::connect(const std::string& host,int port)
{
    //sin_addr.s_addr =  adresse IP
    sock_cfg.sin_addr.s_addr = inet_addr(host.c_str());
    //sin_port = port à utiliser
    sock_cfg.sin_port = htons(port);

    return connect();
};

bool Socket::connect(int port)
{
    //sin_addr.s_addr =  adresse IP
    sock_cfg.sin_addr.s_addr = htonl(INADDR_ANY);
    //sin_port = port à utiliser
    sock_cfg.sin_port = htons(port);

    return connect();
}

bool Socket::connect()
{
    if(need_connect)
    {
        if(::connect(sock, (SOCKADDR*)&sock_cfg, sizeof(sockaddr)) != SOCKET_ERROR)
        {
            std::cerr<<"<id:"<<sock<<">Connected to "<<inet_ntoa(sock_cfg.sin_addr)<<":"<<htons(sock_cfg.sin_port)<<std::endl;
            return true;
        }
        else
        {
            std::cerr<<"<id:"<<sock<<">Ennable to connect"<<std::endl;
            return false;
        }
    }
    return true;
}

void Socket::bind()
{
    if(::bind(sock,(SOCKADDR*)&sock_cfg,sizeof(sock_cfg)) == SOCKET_ERROR)
    {
        perror("bind()");
        throw SocketExeption("Ennable to bind soket");
    }
}

void Socket::listen(const int max_connexion)
{
    if(::listen(sock,max_connexion) == SOCKET_ERROR)
    {
        perror("listen()");
        throw SocketExeption("Ennable to listen");
    }
}

void Socket::serverMode(int port,const int max_connexion,std::string host)
{
    //sin_addr.s_addr = adresse IP à utiliser
    //IP automatiquement chopée
    if(host.size() == 0)
        sock_cfg.sin_addr.s_addr = htonl(INADDR_ANY);
    else
        sock_cfg.sin_addr.s_addr= inet_addr(host.c_str());

    //sin_port = port à utiliser
    sock_cfg.sin_port = htons(port);

    bind();
    listen(max_connexion);

};

Socket Socket::accept()
{
    Socket client(true);
    accept(client);
    return client;
};

void Socket::accept(Socket& client)
{
    socklen_t size = sizeof(sockaddr_in);
    client.sock = ::accept(sock,(sockaddr*) &(client.sock_cfg), &size);
    if (client.sock == INVALID_SOCKET)
    {
        perror("accept()");
        throw SocketExeption("Invalid socket get");
    }
    std::cerr<<"<id:"<<sock<<">New connexion accepted <id:"<<client.sock<<"> from "<<inet_ntoa(client.sock_cfg.sin_addr)<<":"<<htons(client.sock_cfg.sin_port)<<std::endl;
};

bool Socket::shutdown(Socket::Down mode)
{
    return ::shutdown(sock,mode) != -1;
};

std::string Socket::getIp() const
{
    return std::string(inet_ntoa(sock_cfg.sin_addr));
}

unsigned int Socket::getPort() const
{
    return htons(sock_cfg.sin_port);
}

bool Socket::setBroadcast(bool enable)
{
    int tmp = enable;
    return ::setsockopt(sock,SOL_SOCKET,SO_BROADCAST,(const char*)&tmp,sizeof(tmp)) == 0;
}

bool Socket::setReusable(bool enable)
{
    int tmp = enable;
    return ::setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,(const char*)&tmp,sizeof(int)) == 0;
}

void Socket::init()
{
    #if __WIN32
    WSAStartup(MAKEWORD(2,0),&Socket::WSAData);
    #endif // __WIN32
}

void Socket::close()
{
     #if __WIN32
    WSACleanup();
    #endif // __WIN32
}

};


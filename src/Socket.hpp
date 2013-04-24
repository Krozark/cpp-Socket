#ifndef SOCKET_HPP
#define SOCKET_HPP

#ifdef _WIN32 //_WIN64

    #include <winsock2.h>

#elif __linux //|| __unix //or __APPLE__ 

    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h> /* close */
    #include <netdb.h> /* gethostbyname */
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    #define closesocket(s) close(s)
    typedef int SOCKET;
    typedef struct sockaddr_in SOCKADDR_IN;
    typedef struct sockaddr SOCKADDR;
    typedef struct in_addr IN_ADDR;

#else

    #error not defined for this platform

#endif

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string>

#define CRLF		"\r\n"
#define BUF_SIZE	1024
#define PORT 3987

namespace ntw {

class SocketSerialized;

class Socket
{
    public:
        enum Dommaine {IP=AF_INET, LOCAL=AF_UNIX};
        enum Type {TCP=SOCK_STREAM, UDP=SOCK_DGRAM};
        enum Down {SEND=0,RECIVE=1,BOTH=2};

        Socket(Dommaine dommaine,Type type,int protocole=0);
        ~Socket();

        Socket& operator=(const Socket&) = delete;

        const SOCKET id(){return sock;}

        void Connect(std::string host,int port=PORT);
        Socket Wait(std::string host="",int port=PORT);
        void Shutdown(Down mode=Down::BOTH);

        template<typename T>
        inline void Send(const T* data,const size_t size,const int flags=0) const
        {
            if(send(sock,data,size,flags) ==  SOCKET_ERROR)
            {
                perror("Sending message fail");
                throw "Sending message fail";
            }
        }

        template<typename T>
        inline void Receive(T* buffer,const size_t size,const int flags=0) const
        {
            recv(sock,buffer,size,flags);
        };



        static int Max_clients;
        static int Buffer_size;

    protected:
        friend class SocketSerialized;
        Socket(){}// intern use only;
        inline void _close(){closesocket(sock);};
        //socket
        SOCKET sock;
        //configuration
        SOCKADDR_IN sock_cfg;

};

};
#endif

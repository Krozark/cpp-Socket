#ifndef NTW_SOCKET_HPP
#define NTW_SOCKET_HPP

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
#include <exception>

#include <Socket/Config.hpp>




namespace ntw {

class SocketExeption: public std::exception
{
    public:
        SocketExeption(std::string error) : msg(error) {};
        ~SocketExeption() throw() {};
        virtual const char* what() const throw()
        {
            return msg.c_str();
        };
    private:
        std::string msg;
};

class SocketSerialized;

class Socket
{
    public:
        //static int max_id;

        enum Dommaine {IP=AF_INET, LOCAL=AF_UNIX};
        enum Type {TCP=SOCK_STREAM, UDP=SOCK_DGRAM};
        enum Down {SEND=0,RECIVE=1,BOTH=2};

        Socket(Dommaine dommaine,Type type,int protocole=0);
        ~Socket();

        Socket& operator=(const Socket&) = delete;

        const SOCKET id(){return sock;}

        bool connect(std::string host,int port);
        void bind();
        void listen(const int max_connexion);
        void serverMode(int port,const int max_connexion=Config::max_connexion,std::string host="");//init sock_cfg + bind + listen

        Socket accept();
        void accept(Socket& client);

        bool shutdown(Down mode=Down::BOTH);

        template<typename T>
        inline void send(const T* data,const size_t size,const int flags=0) const
        {
            if(::send(sock,data,size,flags) ==  SOCKET_ERROR)
            {
                perror("Send()");
                throw SocketExeption("Sending message fail");
            }
        }

        template<typename T>
        inline int receive(T* buffer,const size_t size,const int flags=0) const
        {
            return ::recv(sock,buffer,size,flags);
        };

        std::string getIp()const;
        unsigned int getPort() const;



    protected:
        friend class SocketSerialized;
        Socket();// intern use only;
        inline void _close(){if(sock != INVALID_SOCKET)closesocket(sock);};
        //socket
        SOCKET sock;
        //configuration
        SOCKADDR_IN sock_cfg;
};

};
#endif

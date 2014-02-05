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

/**
 * \brief A class to manage exception
 */
class SocketExeption: public std::exception
{
    public:
        /**
         * \brief Constructor
         * \param error Message to display
         */
        SocketExeption(std::string error) : msg(error) {};
        ~SocketExeption() throw() {};
        virtual const char* what() const throw()
        {
            return msg.c_str();
        };
    private:
        std::string msg; ///< the message
};

class SocketSerialized;

/**
 * \brief A class tha encapsulate C socket, and made it mor easy to use
 */
class Socket
{
    public:
        //static int max_id;

        /**
         * \brief Named Dommaine type. Others can be used
         */
        enum Dommaine {IP=AF_INET, LOCAL=AF_UNIX};

        /**
         * \brief named Type of socket. Others can be used
         */
        enum Type {TCP=SOCK_STREAM, UDP=SOCK_DGRAM};

        /**
         * \brief Named Down mode
         */
        enum Down {SEND=0,RECIVE=1,BOTH=2};

        /**
         * \brief Constructor
         * \param dommaine the dommaine for the socket
         * \param type the type of socket
         * \param protocole the protocal to use default is 0
         */
        Socket(Dommaine dommaine,Type type,int protocole=0);

        /**
         * \brief Destructor
         */
        ~Socket();

        Socket& operator=(const Socket&) = delete;

        /**
         * \return get the id of the socket
         */
        const SOCKET id()const{return sock;}

        /**
         * \brief connect the socket to the host
         * \param host host adresse (ip or adresse)
         * \param port the port to use
         */
        bool connect(const std::string& host,int port);

        /**
         * \brief connect the socket to the host
         * \param port the port to use
         */
        bool connect(int port);

        /**
         * \brief connect the socket to the host
         */
        bool connect();

        /**
         * \brief Bind the socket
         */
        void bind();

        /**
         * \brief call the listen C function
         */
        void listen(const int max_connexion);

        /**
         * \brief Short cut function to call bind and listen
         * \param port the port to use
         * \param max_connexion the number of client
         * \param host the host to use. default is INADDR_ANY
         */
        void serverMode(int port,const int max_connexion=Config::max_connexion,std::string host="");

        /**
         * \brief Creat a new soket and call accept(Socket&) on it
         * \return the new connextion etablish
         */
        Socket accept();

        /**
         * \brief accept a new connection
         * \param client the new client
         */
        void accept(Socket& client);

        /**
         * \brief shutdown the socket
         * \param mode is the mod to close
         */
        bool shutdown(Down mode=Down::BOTH);

        /**
         * \brief Send data
         * \param data the buffer to send
         * \param size the buffer size
         * \param flags the flags to use
         */
        template<typename T>
        inline int send(const T* data,const size_t size,const int flags=0) const
        {
            int res;
            if(need_connect)
            {
                res = ::send(sock,data,size,flags);
            }
            else
            {
                res = ::sendto(sock,data,size,flags,(SOCKADDR*)(&sock_cfg),sizeof(sock_cfg));
            }

            if(res ==  SOCKET_ERROR)
            {
                perror("Send()");
                throw SocketExeption("Sending message fail");
            }
            return res;
        }

        /**
         * \brief Send data
         * \param data the buffer to send
         * \param size the buffer size
         * \param flags the flags to use (use 0 by default)
         * \param other the host to send datas
         */

        template<typename T>
        inline int send(const T* data,const size_t size,const int flags,const Socket& other) const
        {
            int res;
            if((res = ::sendto(sock,data,size,flags,(SOCKADDR*)(&other.sock_cfg),sizeof(other.sock_cfg))) ==  SOCKET_ERROR)
            {
                perror("Send()");
                throw SocketExeption("Sending message fail");
            }
            return res;
        }



        /**
         * \brief receive data and stor it to the buffer
         * \param buffer the buffer to store the data receive
         * \param size the size of data to receive
         * \param flags the flags to use
         */
        template<typename T>
        inline int receive(T* buffer,const size_t size,const int flags=0) const
        {
            int res;
            if(need_connect)
            {
                res = ::recv(sock,buffer,size,flags);
            }
            else
            {
                socklen_t slen=sizeof(SOCKADDR_IN);
                SOCKADDR_IN rem_cfg; ///< configuration struct
                res = ::recvfrom(sock,buffer,size,flags,(SOCKADDR*)(&rem_cfg),&slen);
            }
            return res;
        };

        /**
         * \brief receive data and store it to the buffer
         * \param buffer the buffer to store the data receive
         * \param size the size of data to receive
         * \param flags the flags to use
         * \param other the other socket mesg from
         */
        template<typename T>
        inline int receive(T* buffer,const size_t size,const int flags,Socket& other) const
        {
            socklen_t slen=sizeof(other.sock_cfg);
            return ::recvfrom(sock,buffer,size,flags,(SOCKADDR*)(&other.sock_cfg),&slen);
        }

        /**
         * \return return the ip
         */
        std::string getIp()const;

        /**
         * \return the port use
         */
        unsigned int getPort() const;


    protected:
        friend class SocketSerialized;
        Socket(bool need_connect);///< intern use only;
        /**
         * \brief close the socket proprely
         */
        inline void _close(){if(sock != INVALID_SOCKET)closesocket(sock);};
        SOCKET sock; ///< C socket type
        SOCKADDR_IN sock_cfg; ///< configuration struct
        const bool need_connect;
};

};
#endif

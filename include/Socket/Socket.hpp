#ifndef NTW_SOCKET_HPP
#define NTW_SOCKET_HPP

#ifdef _WIN32 //_WIN64

    #include <winsock2.h>

    #define MSG_NOSIGNAL 0
    typedef int socklen_t;

#elif __linux //|| __unix //or __APPLE__

    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h> /* close */
    #include <netdb.h> /* gethostbyname */
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    #define closesocket(s) ::close(s)
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

#include <iostream>


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
        virtual ~Socket();

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
        void serverMode(int port,const int max_connexion,std::string host="");

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
        inline int send(const T* data,const size_t size,const int flags=MSG_NOSIGNAL) const
        {
            int res;
            if(need_connect)
            {
                res = ::send(sock,(const char*)data,size,flags);
            }
            else
            {
                res = ::sendto(sock,(const char*)data,size,flags,(SOCKADDR*)(&sock_cfg),sizeof(sock_cfg));
            }

            if(res <0)
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
            if((res = ::sendto(sock,(const char*)data,size,flags,(SOCKADDR*)(&other.sock_cfg),sizeof(other.sock_cfg))) ==  SOCKET_ERROR)
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
                res = ::recv(sock,(char*)buffer,size,flags);
            }
            else
            {
                SOCKADDR_IN from = {0}; ///< configuration struct
                socklen_t from_size=sizeof(from);
                res = ::recvfrom(sock,(char*)buffer,size,flags,(SOCKADDR*)(&from),&from_size);
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
            return ::recvfrom(sock,(char*)buffer,size,flags,(SOCKADDR*)(&other.sock_cfg),&slen);
        }

        /**
         * \return return the ip
         */
        std::string getIp()const;

        /**
         * \return the port use
         */
        unsigned int getPort() const;

        /**
        * \brief initialise the context
        */
        static void init();

        /**
        * \brief close the context
        *  put FuncWrapper::srv::callback_dispatch to nullptr.
        */
        static void close();


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

    private:
        #if __WIN32
        static WSADATA WSAData;
        #endif
};

};
#endif

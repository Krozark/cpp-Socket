#ifndef NTW_SOCKETSERIALIZED_HPP
#define NTW_SOCKETSERIALIZED_HPP

#include <Socket/Socket.hpp>
#include <Socket/Serializer.hpp>

namespace ntw {

/**
 * \brief A class tha merge Socket and Serializer.
 * Use the << and >> operators to add or extract data in the socket.
 */
class SocketSerialized : public Serializer, public Socket
{
    public:
        /**
         * \brief Constructor
         * \param dommaine socket dommaine
         * \param type socket type
         * \param protocole socket protocol
         */
        SocketSerialized(Socket::Dommaine dommaine,Socket::Type type,int protocole=0);

        /**
         * \brief Move constructor
         */
        explicit SocketSerialized(Socket&& s);
        //~SocketSerialized();

        /// SOCKET
        //inline void Connect(std::string host,int port=PORT){Socket::Connect(host,port);};
        //inline Socket Wait(std::string host="",int port=PORT){return Socket::Wait(host,port);};
        //inline void Shutdown(Socket::Down mode=Socket::Down::BOTH){Socket::Shutdown(mode);};

        /**
         * \brief Send data in the buffer
         */
        int send();

        /**
         * \brief Send data in the buffer to a specifique dest
         * \param dest the destination
         */
        //int send(const Socket& dest);


        /**
         * \brief store data into the internal buffer
         */
        int receive();

        /**
         * \brief store data into the internal buffer
         * \param src set the IP / port of the source
         */
        //int receive(Socket& src);


        /**
         * set the status code
         */
        void setStatus(short int status);

        /**
         * \return the status code
         */
        short int getStatus()const;

        /// SERIALIZE
        /**
         * \brief clear the internal buffer.
         */
        void clear();

        /**
         * \brief Short cut function tha call send() and clear()
         */
        void inline sendCl(){send();clear();};

        SocketSerialized(const SocketSerialized&) = delete;
        SocketSerialized& operator=(const SocketSerialized&) = delete;

        friend std::ostream& operator<<(std::ostream& output,const SocketSerialized& self); ///<print each byt by his int value between <>


    private:
        //bool is_send;
        short int status;///< status header default is 0

        /**
         * \brief init the buffer header
         * \return the size to send
         */
        //int init_send();


        /**
         * \brief init the size, status and buffer resize
         * \return -1 if error, else the msg size
         */
        //int init_receive();

};

};
#endif

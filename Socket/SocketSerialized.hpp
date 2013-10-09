#ifndef NTW_SOCKETSERIALIZED_HPP
#define NTW_SOCKETSERIALIZED_HPP

#include <Socket/Socket.hpp>
#include <Socket/Serializer.hpp>

namespace ntw {

class SocketSerialized : public Serializer, public Socket
{
    public:
        SocketSerialized(Socket::Dommaine dommaine,Socket::Type type,int protocole=0);
        explicit SocketSerialized(Socket&& s);
        ~SocketSerialized();

        /// SOCKET
        //inline void Connect(std::string host,int port=PORT){Socket::Connect(host,port);};
        //inline Socket Wait(std::string host="",int port=PORT){return Socket::Wait(host,port);};
        //inline void Shutdown(Socket::Down mode=Socket::Down::BOTH){Socket::Shutdown(mode);};

        void send();
        int receive();

        /// SERIALIZE
        void clear();

        void inline sendCl(){send();clear();};

        SocketSerialized(const SocketSerialized&) = delete;
        SocketSerialized& operator=(const SocketSerialized&) = delete;


    private:
        //bool is_send;

};

};
#endif

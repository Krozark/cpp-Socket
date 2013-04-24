#include "Socket.hpp"
#include "SocketSerialized.hpp"

#include <iostream>

class Test
{
    public:
        int i1;
        int i2;
        double d1;
        
        friend ntw::Serializer& operator<<(ntw::Serializer& output,const Test& self)
        {
            output<<self.i1<<self.i2<<self.d1;
            return output;
        };
};

int main(int argc, char* argv[])
{
    /*
    ntw::Socket sock(ntw::Socket::Dommaine::IP,ntw::Socket::Type::TCP);
    ntw::Socket client = sock.Wait();
    char msg[32] = "hello word!";
    std::cout<<"Envoi du message: "<<msg<<std::endl;
    client.Send(msg,32);
    client.Shutdown();
    */
    ntw::SocketSerialized sockSer(ntw::Socket::Dommaine::IP,ntw::Socket::Type::TCP);
    ntw::SocketSerialized clientSer(sockSer.Wait());

    std::cout<<"Envoi du message: <hello word!> et de la stuctur Test{12,13,14.5}"<<std::endl;
    clientSer<<"hello word!";
    Test t =  {12,13,14.5f};
    clientSer<<t;

    std::cout<<clientSer<<std::endl;

    
    clientSer.Send();
    clientSer.Shutdown(ntw::Socket::Down::SEND);


    return 0;
};



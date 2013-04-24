#include "SocketSerialized.hpp"
#include "SelectManager.hpp"

#include <iostream>
#include <chrono>

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

void newclient(ntw::SelectManager& selector,ntw::Socket& sock)
{
    ntw::SocketSerialized clientSock(sock.Accept());

    std::cout<<"Envoi du message: <hello word!> et de la stuctur Test{12,13,14.5}"<<std::endl;
    clientSock<<"hello word!";
    Test t =  {12,13,14.5f};
    clientSock<<t;

    std::cout<<clientSock<<std::endl;

    clientSock.Send();
    clientSock.Shutdown(ntw::Socket::Down::SEND);

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
    sockSer.ServeurMode();

    ntw::SelectManager serverSelector;
    serverSelector.SetRead(true);
    serverSelector.OnSelect = newclient;
    serverSelector.Add(&sockSer);

    serverSelector.Start();
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    serverSelector.Stop();

    return 0;
};



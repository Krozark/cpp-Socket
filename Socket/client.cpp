#include <Socket/SocketSerialized.hpp>
#include <Socket/SelectManager.hpp>

#define NTW_MODE NTW_CLIENT
#include <Socket/FuncWrapper.hpp>

#include <iostream>
#include <chrono>

void reply(ntw::SelectManager& selector,ntw::Socket& sock)
{
    ntw::SocketSerialized& clientSock = *(ntw::SocketSerialized*)&sock;
    if (clientSock.receive() > 0)
    {
        std::cout<<clientSock<<std::endl;
        /*char* c=0;
        clientSock>>c;
        std::cout<<"[client] recu char*: <"<<c<<">"<<std::endl;
    
        clientSock.clear();
        clientSock<<"message du client";

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        clientSock.send();*/
    }
    else
    {
        std::cerr<<"Server connection lost"<<std::endl; 
        selector.remove(&sock);
        selector.stop();
    }
};

int main(int argc, char* argv[])
{
    /*
    ntw::Socket sock(ntw::Socket::Dommaine::IP,ntw::Socket::Type::TCP);
    sock.Connect("127.0.0.1");
    char msg[32] = {0};
    sock.Receive(msg,32);
    std::cout<<"Recu : "<<msg<<std::endl;
    */

    /*
    ntw::Serializer ser(0);
    char a[]="test de la mort";
    ser<<'t'
        <<65
        <<67
        <<a
        <<68;

    char c;
    ser>>c;
    std::cout<<c<<std::endl;

    int i;
    ser>>i;
    std::cout<<i<<std::endl;

    ser>>i;
    std::cout<<i<<std::endl;

    char* s = NULL;

    ser>>s;
    std::cout<<s<<std::endl;
    */
    
    ntw::SocketSerialized sock(ntw::Socket::Dommaine::IP,ntw::Socket::Type::TCP);
    sock.connect("127.0.0.1");

    /*ntw::SelectManager clientSelector;
    clientSelector.setRead(true);
    clientSelector.onSelect = reply;
    clientSelector.add(&clientSock);

    clientSelector.start();
    clientSelector.wait();*/

    if (not ntw::FuncWrapper::verifyConnect(sock))
    {
        return 1;
    }
    ntw::FuncWrapper::getVersion(sock);

    return 0;
};



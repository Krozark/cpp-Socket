#include <Socket/SocketSerialized.hpp>
#include <Socket/SelectManager.hpp>

#define NTW_MODE NTW_SERVER
#include <Socket/FuncWrapper.hpp>

#include <iostream>
#include <chrono>

ntw::SelectManager clientSelector;

void newclient(ntw::SelectManager& selector,ntw::Socket& sock)
{
    ntw::SocketSerialized* clientSock = new ntw::SocketSerialized(sock.accept());
    ntw::FuncWrapper::verifyConnect(*clientSock);

    //clientSock->Shutdown(ntw::Socket::Down::SEND);
    clientSelector.add(clientSock);
};

void reply(ntw::SelectManager& selector,ntw::Socket& sock)
{
    ntw::SocketSerialized& clientSock = *(ntw::SocketSerialized*)&sock;
    std::cout<<"Répond à "<<clientSock.id()<<std::endl;
    if(clientSock.receive() >0)
    {
        /*char* c=0;
        clientSock>>c;
        std::cout<<"[serveur] recu char*: <"<<c<<">"<<std::endl;
        clientSock.clear();
        clientSock<<"message du serveur";
        clientSock.send();*/
        ntw::FuncWrapper::dispatch(clientSock);
    }
    else
    {
        std::cerr<<"Client connection lost <id:"<<clientSock.id()<<">"<<std::endl; 
        selector.remove(&clientSock);
        delete &clientSock;
    }
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
    sockSer.serveurMode();

    ntw::SelectManager serverSelector;
    serverSelector.setRead(true);
    serverSelector.onSelect = newclient;
    serverSelector.add(&sockSer);

    clientSelector.setRead(true);
    clientSelector.onSelect = reply;

    clientSelector.start();
    serverSelector.start();
    //std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    //serverSelector.Stop();

    clientSelector.wait();
    serverSelector.wait();

    return 0;
};



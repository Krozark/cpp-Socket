#include <Socket/server/Srv-Server.hpp>

namespace ntw
{
namespace srv
{
    Serveur::Serveur(unsigned int max_client,unsigned int min_client,float timeout) : new_connexion_recv(timeout),
    request_recv(true,false,false,onRequestRecv,min_client,max_client,0,timeout),
    broadcast_sender(true,false,false,onBroadCastRecv,min_client,max_client,0,timeout)
    {
        new_connexion_recv.onSelect = onNewClientRecv;
    }

    void Serveur::start()
    {
        //start
        new_connexion_recv.start();
        request_recv.start();
        broadcast_sender.start();
        //join
        new_connexion_recv.wait();
        request_recv.wait();
        broadcast_sender.wait();
    }

    void Serveur::stop()
    {
        new_connexion_recv.stop();
        request_recv.stop();
        broadcast_sender.stop();
    }
    void Serveur::onNewClientRecv(ntw::SelectManager& new_connexion_recv, ntw::Socket& sock)
    {
    }

    void Serveur::onRequestRecv(ntw::SelectManager& new_connexion_recv, ntw::Socket& sock)
    {
    }

    void Serveur::onBroadCastRecv(ntw::SelectManager& new_connexion_recv, ntw::Socket& sock)
    {
    }
}
}

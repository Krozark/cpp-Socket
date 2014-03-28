#include "functions.hpp"

void recv_msg(ntw::SocketSerialized& sock,std::string msg)
{
    std::cout<<"[RECV_MSG<"<<sock.id()<<">]"<<msg<<std::endl;
}


#include <Socket/server/Client.hpp>

void register_client(ntw::srv::Server& self,ntw::srv::Client& client)
{
    std::cout<<"[NEW CLIENT] ip:"<<client.sock().getIp()
        <<" port:"<<client.sock().getPort()
        <<std::endl;
}


void unregister_client(ntw::srv::Server& self,ntw::srv::Client& client)
{
    std::cout<<"[DELETE CLIENT] ip:"<<client.sock().getIp()
        <<" port:"<<client.sock().getPort()
        <<std::endl;
}

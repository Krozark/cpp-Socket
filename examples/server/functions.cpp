#include "functions.hpp"

#include <Socket/FuncWrapper.hpp>

int dispatch(int id,ntw::SocketSerialized& request)
{
    int res= ntw::FuncWrapper::Status::st::wrong_id;
    std::cout<<"[dispatch] id:"<<id<<std::endl<<std::flush;
    switch(id)
    {
        case FUNCTION_ID::RECV_MSG:
        {
            res = ntw::FuncWrapper::srv::exec(recv_msg,request);
        }break;
        case FUNCTION_ID::TEST_INPUT:
        {
            res = ntw::FuncWrapper::srv::exec(testinput,request);
        }break;
        default:
        {
            std::cout<<"[dispatch] Function id not found"<<std::endl;
        }break;
    }
    return res;
}

void recv_msg(ntw::SocketSerialized& sock,std::string msg)
{
    std::cout<<"[RECV_MSG<"<<sock.id()<<">]"<<msg<<std::endl;
}

void testinput(ntw::SocketSerialized& sock)
{
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

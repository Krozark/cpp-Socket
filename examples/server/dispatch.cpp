#include "dispatch.hpp"

#include <Socket/FuncWrapper.hpp>

#include "functions.hpp"

namespace ntw
{
    int dispatch(int id,SocketSerialized& request)
    {
        int res= FuncWrapper::Status::st::wrong_id;
        std::cout<<"[dispatch] id:"<<id<<std::endl<<std::flush;
        switch(id)
        {
            case FUNCTION_ID::RECV_MSG:
            {
                res = ntw::FuncWrapper::srv::exec(recv_msg,request);
            }break;
            default:
            {
                std::cout<<"[dispatch] Function id not found"<<std::endl;
            }break;
        }
        return res;
    }
}

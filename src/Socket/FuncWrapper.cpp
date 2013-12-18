#include <Socket/FuncWrapper.hpp>

namespace ntw
{
    int FuncWrapper::msg(SocketSerialized& sock,const std::string message,unsigned int code)
    {
        sock.clear();
        sock<<message<<code;
        sock.sendCl();
        return NTW_ERROR_NO;
    }

    int FuncWrapper::verifyIsConnected(SocketSerialized& sock)
    {
        int code = NTW_ERROR_UNKNOW; 
        if(sock.receive() > 0)
        {
            std::string msg;
            sock>>msg>>code;

            if(std::string(msg) == std::string(NTW_WELCOM_MSG) and code == NTW_ERROR_NO)
            {
                std::cerr<<"verifyIsConnected <"<<code<<":"<<msg<<">"<<std::endl;
            }
            else
            {
                std::cerr<<"verifyIsConnected <"<<code<<":"<<msg<<">"<<std::endl;
            }
            sock.clear();
        }
        return code;
    }

    /********** CLIENT ********************/
    void FuncWrapper::cli::addPackage(SocketSerialized& sock)
    {
    }

    /*********** SERVER ******************/
    void FuncWrapper::srv::dispatch(SocketSerialized& request)
    {
        int id = FUNCTONS_ID::UNKNOW;
        request>>id;

        if(ntw::dispatch(id,request) == Status::st::wrong_id)
        {
            request.clear();
            request<<ntw::FuncWrapper::Status(Status::st::wrong_id);
            request.sendCl();
        }
    }
}
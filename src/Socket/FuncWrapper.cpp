#include <Socket/FuncWrapper.hpp>
#include <Socket/define.hpp>


namespace ntw
{
    //int (*FuncWrapper::srv::callback_dispatch)(int id,SocketSerialized& request) = nullptr;

    int FuncWrapper::msg(SocketSerialized& sock,const std::string message,unsigned int code)
    {
        sock.clear();
        sock.setStatus(code);
        sock<<message;
        sock.sendCl();
        return Status::ok;
    }

    /********** CLIENT ********************/
    void FuncWrapper::cli::addPackage(SocketSerialized& sock)
    {
    }

    /*********** SERVER ******************/
    void FuncWrapper::srv::dispatch(SocketSerialized& request,int(*callback_dispatch)(int id,SocketSerialized&))
    {
        int id = FUNCTONS_ID::UNKNOW;
        request>>id;

        if(callback_dispatch(id,request) == Status::wrong_id)
        {
            request.clear();
            request.setStatus(Status::wrong_id);
            request.sendCl();
        }
    }

    int FuncWrapper::cli::verifyIsConnected(SocketSerialized& sock)
    {
        short int code;
        if(sock.receive() > 0)
        {
            std::string msg;
            sock>>msg;
            code = sock.getStatus();

            if(std::string(msg) == std::string(NTW_WELCOM_MSG) and code == Status::ok)
            {
                std::cerr<<"verifyIsConnected <"<<code<<":"<<msg<<">"<<std::endl;
            }
            else
            {
                std::cerr<<"verifyIsConnected <"<<code<<":"<<msg<<">"<<std::endl;
            }
            sock.clear();
        }
        else
        {
            code = sock.getStatus();
        }
        return code;
    }

    template<>
    void FuncWrapper::cli::send_f<void>(SocketSerialized& sock)
    {
        if (sock.receive() > 0)
        {
            if(sock.getStatus() != ntw::Status::wrong_id)
            {
            }
            else
            {
                std::cerr<<"Recive Status different of \"ntw::Status::ok\""<<std::endl;
            }
        }
        return;
    }

}

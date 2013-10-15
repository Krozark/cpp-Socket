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

#if NTW_MODE == NTW_CLIENT
    /***************************
     ********** CLIENT**********
     **************************/
    int FuncWrapper::getVersion(SocketSerialized& sock)
    {
        int v=0;
        addPackage(GET_VERSION,sock);
        sock.send();
        if (sock.receive() > 0)
        {
            sock>>v;
        }
        return v;
    }

    void FuncWrapper::addPackage(SocketSerialized& sock)
    {
    }
#else
    /***************************
     * ****** SERVER **********
     * ************************/
    int FuncWrapper::getVersion(SocketSerialized& sock)
    {
        sock.clear();
        sock<<NTW_VERSION;
        sock.sendCl();        
    }

    void FuncWrapper::dispatch(SocketSerialized& request)
    {
        while(request.size()>0)
        {
            int id = FUNCTONS_ID::UNKNOW;
            request>>id;

            switch(id)
            {
                case FUNCTONS_ID::UNKNOW :
                {
                    std::cerr<<"[ERROR] FuncWrapper::dispatch, FUNCTONS_ID UNKNOW"<<std::endl;
                }break;
                case FUNCTONS_ID::GET_VERSION :
                {
                    getVersion(request);
                }break;
                default:
                    std::cerr<<"[ERROR] FuncWrapper::dispatch, FUNCTONS_ID not find"<<std::endl;
            }
        }
    }
#endif

}

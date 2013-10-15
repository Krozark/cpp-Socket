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
    void FuncWrapper::addPackage(SocketSerialized& sock){}

    int FuncWrapper::getVersion(SocketSerialized& sock)
    {
        return send<int>(sock,GET_VERSION);
    }
    int FuncWrapper::testParamInt(SocketSerialized& sock,int p1)
    {
        return send<int>(sock,TESTPARAMINT,p1);
    }


#else
    /***************************
     * ****** SERVER **********
     * ************************/

    int FuncWrapper::getVersion(SocketSerialized& sock)
    {
        sock<<NTW_VERSION;
    }

    int FuncWrapper::testParamInt(SocketSerialized& sock,int v)
    {
        std::cout<<"param:"<<v<<std::endl;
        sock<<12;
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
                    request.clear();
                    getVersion(request);
                    request.sendCl();        
                }break;
                case  FUNCTONS_ID::TESTPARAMINT :
                {
                    int a = 0;
                    request>>a;
                    request.clear();
                    testParamInt(request,a);
                    request.sendCl();        
                }
                default:
                    std::cerr<<"[ERROR] FuncWrapper::dispatch, FUNCTONS_ID not find"<<std::endl;
            }
        }
    }


#endif

}

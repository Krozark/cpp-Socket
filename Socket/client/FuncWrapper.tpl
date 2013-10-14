namespace ntw
{
    int FuncWrapper::getVersion(SocketSerialized& sock)
    {
        int v=0;
        addPackage(GET_VERSION,sock);
        sock.send();
        if (sock.receive() > 0)
        {
            sock>>v;
            std::cout<<v<<std::endl;
        }
        return v;
    }

    bool FuncWrapper::verifyConnect(SocketSerialized& sock,const std::string message,unsigned int code)
    {
        bool res=false;
        if(sock.receive() > 0)
        {
            char* msg = 0;
            sock>>msg;
            sock>>code;

            if(std::string(msg) == std::string(message) and code == NTW_ERROR_NO)
            {
                sock.clear();
                res = true;
            }
            else
            {
                std::cerr<<"[client] rcv <"<<code<<":"<<msg<<">"<<std::endl;
            }
        }
        return res;
    }


    template<typename ... Args>
    void FuncWrapper::addPackage(FUNCTONS_ID id,SocketSerialized& sock,Args& ... args)
    {
        sock<<id;
        addPackage(sock,args...);
    }


    template<typename T,typename ... Args>
    void FuncWrapper::addPackage(SocketSerialized& sock,T& a,Args& ... args)
    {
        sock<<a;
        addPackage(sock,args...);
    }

    void FuncWrapper::addPackage(SocketSerialized& sock)
    {
    }
}

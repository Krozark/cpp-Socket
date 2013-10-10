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

    bool FuncWrapper::verifyConnect(SocketSerialized& sock)
    {
        bool res=false;
        if(sock.receive() > 0)
        {
            char* msg = 0;
            sock>>msg;
            std::cout<<"[client] recu char*: <"<<msg<<">"<<std::endl;
            if(std::string(msg) == std::string(NTW_WELCOM_MSG))
            {
                sock.clear();
                res = true;
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

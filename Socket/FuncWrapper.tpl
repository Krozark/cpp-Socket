namespace ntw
{
#if NTW_MODE == NTW_CLIENT
    /***************************
     ********** CLIENT**********
     **************************/
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

    template<typename Ret,typename ... Args>
    Ret FuncWrapper::send(SocketSerialized& sock,FUNCTONS_ID id,Args&& ... args)
    {
        Ret ret;
        addPackage(id,sock,args ...);
        sock.send();
        if (sock.receive() > 0)
        {
            sock>>ret;
        }
        return ret;
    }
#else
    /***************************
     * ****** SERVER **********
     * ************************/
    template<typename Ret,typename ... Args>
    Ret send(Ret(*func)(SocketSerialized& sock,Args&& ...),SocketSerialized& sock)
    {
    }
#endif
}

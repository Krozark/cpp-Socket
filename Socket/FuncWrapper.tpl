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
#else
    /***************************
     * ****** SERVER **********
     * ************************/
#endif
}

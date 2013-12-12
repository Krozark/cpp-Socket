#ifndef NTW_FUNCWRAPPER
#define NTW_FUNCWRAPPER

#include <Socket/SocketSerialized.hpp>
#include <Socket/define.hpp>


namespace ntw
{
    extern void dispatch(int id,SocketSerialized& request);

    class FuncWrapper
    {
        public:
            FuncWrapper() = delete;
            FuncWrapper(const FuncWrapper&) = delete;
            FuncWrapper& operator=(const FuncWrapper&) = delete;

            static int msg(SocketSerialized& sock,const std::string message,unsigned int code); ///< initialise the socket (client : verify the msg, serveur: send hello)
            static int verifyIsConnected(SocketSerialized& sock);

            class srv 
            {
                public:
                    enum FUNCTONS_ID{UNKNOW=-1};
                    static void dispatch(SocketSerialized& request);

                    template<typename Ret,typename ... Args>
                    static Ret exec(Ret(*func)(SocketSerialized&,Args ...),SocketSerialized& sock);

                private:
                    srv() = delete;
                    srv& operator=(const srv&)=delete;
            };
            
            class cli 
            {
                public:
                    template<typename ... Args>
                    static void addPackage(int id,SocketSerialized& sock,Args& ... args);

                    template<typename T,typename ... Args>
                    static void addPackage(SocketSerialized& sock,T& a,Args& ... args);

                    static void addPackage(SocketSerialized& sock);


                    template<typename Ret,typename ... Args>
                    static Ret send(SocketSerialized& sock,int id,Args&& ... args);

                private:
                    cli() = delete;
                    cli& operator=(const cli&) = delete;
            };
    };
};

#include <Socket/FuncWrapper.tpl>

#endif

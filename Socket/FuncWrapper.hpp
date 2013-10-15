#ifndef NTW_FUNCWRAPPER
#define NTW_FUNCWRAPPER

#include <Socket/SocketSerialized.hpp>
#include <Socket/define.hpp>


namespace ntw
{
    class FuncWrapper
    {
        public:
            FuncWrapper() = delete;
            FuncWrapper(const FuncWrapper&) = delete;
            FuncWrapper& operator=(const FuncWrapper&) = delete;

            static int msg(SocketSerialized& sock,const std::string message,unsigned int code); ///< initialise the socket (client : verify the msg, serveur: send hello)
            static int verifyIsConnected(SocketSerialized& sock);

            static int getVersion(SocketSerialized& sock); ///< get serveur version
            static int testParamInt(SocketSerialized& sock,int);

            enum FUNCTONS_ID {UNKNOW=0,GET_VERSION,TESTPARAMINT,MAX_FN_ID};

            #if NTW_MODE == NTW_SERVER
            static void dispatch(SocketSerialized& request);
            #endif
            
        protected:
            #if NTW_MODE == NTW_CLIENT
            template<typename ... Args>
            static void addPackage(FUNCTONS_ID id,SocketSerialized& sock,Args& ... args);

            template<typename T,typename ... Args>
            static void addPackage(SocketSerialized& sock,T& a,Args& ... args);

            static void addPackage(SocketSerialized& sock);


            template<typename Ret,typename ... Args>
            static Ret send(SocketSerialized& sock,FUNCTONS_ID id,Args&& ... args);
            #else

            template<typename Ret,typename ... Args>
            static Ret send(Ret(*func)(SocketSerialized& sock,Args&& ...),SocketSerialized& sock);
            #endif


    };
};

#include <Socket/FuncWrapper.tpl>

#endif

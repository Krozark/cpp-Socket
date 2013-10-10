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

            static int getVersion(SocketSerialized& sock); ///< get serveur version
            static bool verifyConnect(SocketSerialized& sock); ///< initialise the socket (client : verify the msg, serveur: send hello)

            enum FUNCTONS_ID {UNKNOW=0,GET_VERSION,MAX_FN_ID};

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
            #endif


    };
};

#if NTW_MODE == NTW_CLIENT
#include <Socket/client/FuncWrapper-client.tpl>
#else
#include <Socket/server/FuncWrapper-serveur.tpl>
#endif

#endif

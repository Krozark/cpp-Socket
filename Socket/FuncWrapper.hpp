#ifndef NTW_FUNCWRAPPER
#define NTW_FUNCWRAPPER

#include "SocketSerialized.hpp"


//#define NTW_MODE NTW_CLIENT

namespace ntw
{
    class FuncWrapper
    {
        public:
            FuncWrapper() = delete;
            FuncWrapper(const FuncWrapper&) = delete;
            FuncWrapper& operator=(const FuncWrapper&) = delete;

            static int getVersion(SocketSerialized& sock);
            //static void connect(SocketSerialized& sock);

            enum FUNCTONS_ID {UNKNOW=0,GET_VERSION,MAX_FN_ID};
            
        //protected:
            #if NTW_MODE == NTW_CLIENT
            template<typename ... Args>
            static void addPackage(FUNCTONS_ID id,SocketSerialized& sock,Args& ... args);

            template<typename T,typename ... Args>
            static void addPackage(SocketSerialized& sock,T& a,Args& ... args);

            static void addPackage(SocketSerialized& sock);


            #else
            static void dispatch(SocketSerialized& request);

            #endif


    };
};

#if NTW_MODE == NTW_CLIENT
#include "FuncWrapper-client.tpl"
#else
#include "FuncWrapper-serveur.tpl"
#endif

#endif

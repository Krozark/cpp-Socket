#ifndef NTW_FUNCWRAPPER
#define NTW_FUNCWRAPPER

#include <Socket/SocketSerialized.hpp>
#include <Socket/define.hpp>


namespace ntw
{
    /**
     * \brief the external function to call whene recv a message
     * \param id the function id
     * \param request the request who send the request
     */
    extern void dispatch(int id,SocketSerialized& request);

    /**
     * \brief Regroup somme helper function for TCP client server call
     */
    class FuncWrapper
    {
        public:
            FuncWrapper() = delete;
            FuncWrapper(const FuncWrapper&) = delete;
            FuncWrapper& operator=(const FuncWrapper&) = delete;

            /**
             * \brief send a message to the sock. Use to send errors
             * \param sock to use
             * \param message to send
             * \param code the error code
             * \return NTW_ERROR_NO
             */
            static int msg(SocketSerialized& sock,const std::string message,unsigned int code);

            /**
             * \brief receive data from sock and check if
             * the message and code recv are the welcome one.
             * \param sock socket to check
             * \return the code recv
             */
            static int verifyIsConnected(SocketSerialized& sock);

            /**
             * \brief Regroup function for build a TCP server
             */
            class srv 
            {
                public:
                    enum FUNCTONS_ID{UNKNOW=-1}; ///< Function namespace
                    static void dispatch(SocketSerialized& request); ///< call the dispatch(int,sock) function with the extracted id of function

                    /***
                     * \brief shortcut function to call a function.
                     * Exctact the param from the socket
                     * Note : sock format is <id|params ...>
                     * \param func the function to call
                     * \param sock the socket containing the param
                     * \return the func value
                     */
                    template<typename Ret,typename ... Args>
                    static Ret exec(Ret(*func)(SocketSerialized&,Args ...),SocketSerialized& sock);

                private:
                    srv() = delete;
                    srv& operator=(const srv&)=delete;
            };
            
            /**
             * \brief A class to help to build a TCP client
             */
            class cli 
            {
                public:
                    /**
                     * \brief Call operator::<< on all args
                     * \param id the function id to call
                     * \param sock the socket used to store args
                     * \params args to copy in sock
                     */
                    template<typename ... Args>
                    static void addPackage(int id,SocketSerialized& sock,Args& ... args);

                    /**
                     * \brief Call operator<< on all args
                     * \param sock the socket used to store args
                     * \params args to copy in sock
                     */
                    template<typename T,typename ... Args>
                    static void addPackage(SocketSerialized& sock,T& a,Args& ... args);

                    /**
                     * \brief do nothing (for break template recurcivity)
                     */
                    static void addPackage(SocketSerialized& sock);


                    /**
                     * \brief call the function of id id on the server with param ... args
                     * \param sock the socket to use
                     * \param id the id function to call on the server
                     * \param args the args of the function to call
                     * \return the function called return value
                     */
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

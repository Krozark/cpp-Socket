#ifndef NTW_FUNCWRAPPER
#define NTW_FUNCWRAPPER

#include <Socket/SocketSerialized.hpp>
#include <Socket/define.hpp>


namespace ntw
{
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
             * \brief A class to handle status response
             */
            class Status
            {
                public:
                    Status(short int c):code(c){}; ///< constructor

                    /*friend Serializer& operator<<(Serializer& stream,const Status& self)
                    {
                        stream<<self.code;
                        return stream;
                    }

                    friend Serializer& operator>>(Serializer& stream,const Status& self)
                    {
                        stream>>self.code;
                        return stream;
                    }*/
                    short int code; ///< code

                    /**
                     * \brief code values
                     */
                    enum st{stop=NTW_STOP_CONNEXION,
                        wrong_id=-2,
                        ok=0,
                    };
            };

            /**
             * \brief Regroup function for build a TCP server
             */
            class srv
            {
                public:
                    enum FUNCTONS_ID{UNKNOW=-1}; ///< Function namespace
                    /**
                     * \brief dispatch function. it call callback_dispatch
                     * Not callback_dispatch must return error code. if the value is Status::st::wrong_id, it send error message to client
                     * \param callback_dispatch the callback to call
                     * \param id the function id
                     * \param request the request who send the request
                     */
                    static void dispatch(SocketSerialized& request,int (*callback_dispatch)(int id,SocketSerialized& request));

                    /***
                     * \brief shortcut function to call a function.
                     * Exctact the param from the socket, and store the result in the socket
                     * Note : sock format is <id|status|params ...>
                     * \param func the function to call. use "return send<T>(sock,id, Args...)";
                     * \param sock the socket containing the param
                     * \return the func socket status
                     */
                    template<typename Ret,typename ... Args>
                    static int exec(Ret(*func)(SocketSerialized&,Args ...),SocketSerialized& sock);


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
                     * \return the function called return value. Verify the status value before use it.
                     */
                    template<typename Ret,typename ... Args>
                    static Ret send(SocketSerialized& sock,int id,Args&& ... args);


                    /**
                     * \brief receive data from sock and check if
                     * the message and code recv are the welcome one.
                     * \param sock socket to check
                     * \return the code recv
                     */
                    static int verifyIsConnected(SocketSerialized& sock);

                private:
                    cli() = delete;
                    cli& operator=(const cli&) = delete;

                    template <typename Ret>
                    static Ret send_f(SocketSerialized& sock);

            };
    };
};

#include <Socket/FuncWrapper.tpl>

#endif

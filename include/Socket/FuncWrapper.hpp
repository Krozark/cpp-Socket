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
     * \return error code. if the value is Status::st::wrong_id, send error message to client
     */
    extern int dispatch(int id,SocketSerialized& request);

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
             * \brief A class to handle status response
             */
            class Status
            {
                public:
                    Status(int c):code(c){}; ///< constructor

                    friend Serializer& operator<<(Serializer& stream,const Status& self)
                    {
                        stream<<self.code;
                        return stream;
                    }

                    friend Serializer& operator>>(Serializer& stream,const Status& self)
                    {
                        stream>>self.code;
                        return stream;
                    }
                    int code; ///< code

                    /**
                     * \brief code values
                     */
                    enum st{wrong_id=-2,
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
                    static void dispatch(SocketSerialized& request); ///< call the dispatch(int,sock) function with the extracted id of function

                    /***
                     * \brief shortcut function to call a function.
                     * Exctact the param from the socket
                     * Note : sock format is <id|status|params ...>
                     * \param func the function to call. use "return send<T>(sock,id,Status& st)";
                     * \param status the status. [Default is ntw::FuncWrapper::Status::ok =>0].
                     * \param sock the socket containing the param
                     * \return the func status
                     */
                    template<typename Ret,typename ... Args>
                    static int exec(Ret(*func)(SocketSerialized&, int& status,Args ...),SocketSerialized& sock);

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
                     * \param st stor the status (sended ok or not)
                     * \param id the id function to call on the server
                     * \param args the args of the function to call
                     * \return the function called return value. Verify the status value before use it.
                     */
                    template<typename Ret,typename ... Args>
                    static Ret send(SocketSerialized& sock,Status& st,int id,Args&& ... args);

                private:
                    cli() = delete;
                    cli& operator=(const cli&) = delete;
            };
    };
};

#include <Socket/FuncWrapper.tpl>

#endif

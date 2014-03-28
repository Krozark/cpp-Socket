cpp-Socket
==========

Current build status : 
[![Build Status](https://travis-ci.org/Krozark/cpp-Socket.png?branch=master)](https://travis-ci.org/Krozark/cpp-Socket)

Testing
=======

Ubuntu 12.04+ x86/64
Windows xp x86


Version : 0.2

Some class to help to construct client/server with soket and serilized datas.

It also make some methode to make binary RPC. The binary is translate using the serializer before the send.

the data are send like this :
|size|function id|param...|
and the responce is like that :
|size|response|

Note:
    It's important to know witch type are the parameters and the order.
    It's the same for the result of the call.
    Some function are build to don't make mistake :

* Client::call(R (\*fun)(SocketSerialized&,Args... ),Args ...)
* Server::exec(R (\*fun)(SocketSerialized&,Args ...),Args...)

Test on Ubuntu 12+ x64, but based on multiplatforme code.
Used std::thread and std::mutex.

Important
=========

You have to define your function dispatch. See the doc for more detail.

```C++

namespace ntw
{
    class SocketSerialized;

    int dispatch(int id,SocketSerialized& request)
    {
        return 0;
    };
}

```


Class
-----

All class are in ntw namespace.

* ntw::Config
    * define some values use for init server, client and SelectManager.
        * max_connexion;
        * port_server;
        * port_client;
        * default_timeout;

* ntw::Serializer
    * allow you to add datas to the buffer that are convert in Big Endian for the network
    * allow you to add custom operators \<\< and \>\>  for easily use vour own class/stuct to convert.

* ntw::Socket
    * use C socket, and put them in a class to use them more easily

* ntw::SocketSerialized
    * Extand Socket and Serializer.
    * Simply add that you want in this soket (using \<\< operator) send it, and get data (with \>\> operator)
    * RPC call are made using this Socket class.

* ntw::SelectManager
    * Manage some Soket (just add some in it) and execute callback whene critera are satisfy (read,write, except)
    * Run in a different thread for performence

* ntw::BalancingSelector
    * A groupe of SelectManager that do the same. It's use to split the number of socket to manage in defferent thread without having to do it.

* ntw::FuncWrapper
    * This class define some (static) base functions for the communication
    * This class use a dispatch function tha you have to build
    * exec is a shortcut tha unpack param of the function in firs param, containing in the request, and call it.

```C++

namespace ntw
{
    void dispatch(int id,SocketSerialized& request)
    {
        switch(id)
        {
            case ntw::FuncWrapper::srv::FUNCTONS_ID::UNKNOW :
            {
                std::cerr<<"[ERROR] dispatch, FUNCTONS_ID UNKNOW"<<std::endl;
            }break;
            case 1 :
            {
                exec(getVersion,request);
            }break;
            case  2 :
            {
                exec(testParamInt,request);
            }break;
            default:
                std::cerr<<"[ERROR] FuncWrapper::dispatch, FUNCTONS_ID not find: "<<id<<std::endl;
        }
    }
}

```

* ntw::srv::Server
    * this is the server implementation
    * Just construct and start it , and that'is it.

* ntw::cli::Client
    * this is the client implemantation
    * construct and connect it, and then, you can use the client.call((*pf)'SocketSerialized&,...)...) to make a rpc call. pf is a fonction defined in FuncWrapper (client.call is a synchronous call).
    * at the end, don't forget to call client.stop(), and client.wait() to stop the network thread.


You can build the lib with cmake.




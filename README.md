cpp-Socket
==========

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
        * Client::call(R (*fun)(SocketSerialized&,Args... ),Args ...)
        * Server::exec(R (*fun)(SocketSerialized&,Args ...),Args...)

Test on Ubuntu 12+ x64, but based on multiplatforme code.

Class
-----

All class are in ntw namespace.

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
    * This is in this class that you have to add you own function (.hpp, .cpp or .tpl for template)
    * Client side : the body of function are juste like that : return send\<ReturnType\>(sock,unique function id,params ...)
    * Server side : normal function, juste make your stuf, and return the result.
    * Server also need to build FuncWrapper::dispatch(SocketSerialized& request) function. It's juste a big switch on the id function. Here is the default:

```c++
void FuncWrapper::dispatch(SocketSerialized& request)
{
    if(request.size()>0)
    {
        int id = FUNCTONS_ID::UNKNOW;
        request>>id;

        switch(id)
        {
            case FUNCTONS_ID::UNKNOW :
            {
                std::cerr<<"[ERROR] FuncWrapper::dispatch, FUNCTONS_ID UNKNOW"<<std::endl;
            }break;
            case FUNCTONS_ID::GET_VERSION :
            {
                exec(getVersion,request);
            }break;
            case  FUNCTONS_ID::TESTPARAMINT :
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

Exemples
========


You can find exemples of use in:
* Socket/server/server.cpp

```c++
#include <Socket/server/Server.hpp>

int main(int argc, char* argv[])
{

    const unsigned int max_client = 100;
    ntw::srv::Server server(max_client);
    server.start();

    return 0;
};
```

* Socket/client/client.cpp

```c++
#include <Socket/client/Client.hpp>
#include <Socket/FuncWrapper.hpp>

#include <iostream>

int main(int argc, char* argv[])
{
    
    ntw::cli::Client client;
    int res = client.connect("127.0.0.1",NTW_PORT_SERVER);

    if(res != NTW_ERROR_NO)
    {
        std::cerr<<"An error occurred (code:"<<res<<")"<<std::endl;
        return res;
    }

    std::cout<<client.call(ntw::FuncWrapper::getVersion)<<std::endl;
    std::cout<<client.call(ntw::FuncWrapper::testParamInt,25)<<std::endl;

    client.stop();
    client.wait();

    return 0;
};
```



You can build the exemple with "make".




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



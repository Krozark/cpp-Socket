#include "functions.hpp"
#include "defines.hpp"

#include <Socket/SocketSerialized.hpp>
#include <Socket/client/Client.hpp>
#include <Socket/Config.hpp>

#include <sstream>

void run(ntw::cli::Client& client)
{
    bool run = true;
    while (run)
    {
        std::string input;
        std::cout<<"Message to send [Q/q to exit]\n>";
        std::cin>>input;

        if (input == "Q" or input == "q")
        {
            run = false;
            break;
        }

        client.call<void>(RECV_MSG,input);

        /*
        ///client.call<void>(RECV_MSG,input); make this:
        {
            client.request_sock.clear();
            client.request_sock.setStatus(ntw::FuncWrapper::Status::ok);
            //set the function id
            client.request_sock<<(int)RECV_MSG
                <<input;
            client.request_sock.send();
            std::cout<<"send data"<<std::endl;
            if(client.request_sock.receive() > 0)
                std::cout<<client.request_sock<<std::endl;
        }
        */

        short int status = client.request_sock.getStatus();
        switch(status)
        {
            case ERRORS::STOP :
            {
                std::cerr<<" : The server is probably down."<<std::endl;
                std::cout<<"[Recv] Stop"<<std::endl
                    <<"The programme will now stop"<<std::endl;
                client.request_sock.clear();
                run = false;
            }break;
            default :
            {
                std::cout<<"[Recv] Server error code:"<<status<<std::endl;
                client.request_sock.clear();
                /// server error???
            }break;
        }
    }
}

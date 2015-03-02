#include <Socket/client/Client.hpp>
#include <Socket/FuncWrapper.hpp>

namespace ntw
{
    namespace cli
    {
        Client::Client() :  request_sock(Socket::Domain::IP,Socket::Type::TCP)
        {
        }

        Client::~Client()
        {
        }

        int Client::connect(const std::string& host,int port)
        {   
            //init request sock
            if(not request_sock.connect(host,port))
                return Status::connexion;

            return ntw::FuncWrapper::cli::verifyIsConnected(request_sock);
        }

        void Client::disconnect()
        {
            request_sock.disconnect();
        }

    }
}

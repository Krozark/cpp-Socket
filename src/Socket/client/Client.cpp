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
            request_sock.connect(host,port);

            if (ntw::FuncWrapper::cli::verifyIsConnected(request_sock) != Status::ok)
            {
                return Status::connexion;
            }
            return Status::ok;
        }

        void Client::disconnect()
        {
            request_sock.disconnect();
        }

    }
}

#include <utility>
#include <Socket/FuncWrapper.hpp>

namespace ntw
{
    namespace cli
    {

        template<typename Ret,typename ... Args>
        Ret Client::call(Ret (*pf)(SocketSerialized&, Args ...),Args&& ... args)
        {
            request_sock.setStatus(ntw::Status::ok);
            return pf(request_sock,std::forward<Args>(args)...);
        }

        template<typename Ret,typename ... Args>
        Ret Client::call(int id,Args&& ... args)
        {
            request_sock.setStatus(ntw::Status::ok);
            return ntw::FuncWrapper::cli::send<Ret,Args...>(request_sock,id,std::forward<Args>(args)...);
        }
    }
}

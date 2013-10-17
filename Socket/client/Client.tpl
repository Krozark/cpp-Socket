#include <utility>

namespace ntw
{
    namespace cli
    {

        template<typename Ret,typename ... Args>
        Ret Client::exec(Ret (*pf)(SocketSerialized&, Args ...),Args&& ... args)
        {
            return pf(request_sock,std::forward<Args>(args)...);
        }
    }
}

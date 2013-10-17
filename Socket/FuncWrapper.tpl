#include <tuple>
#include <type_traits>

namespace ntw
{
#if NTW_MODE == NTW_CLIENT
    /***************************
     ********** CLIENT**********
     **************************/
    template<typename ... Args>
    void FuncWrapper::addPackage(FUNCTONS_ID id,SocketSerialized& sock,Args& ... args)
    {
        sock<<id;
        addPackage(sock,args...);
    }


    template<typename T,typename ... Args>
    void FuncWrapper::addPackage(SocketSerialized& sock,T& a,Args& ... args)
    {
        sock<<a;
        addPackage(sock,args...);
    }

    template<typename Ret,typename ... Args>
    Ret FuncWrapper::send(SocketSerialized& sock,FUNCTONS_ID id,Args&& ... args)
    {
        Ret ret;
        addPackage(id,sock,args ...);
        sock.send();
        if (sock.receive() > 0)
        {
            sock>>ret;
        }
        return ret;
    }
#else
    /***************************
     * ****** SERVER **********
     * ************************/
// ------------- UTILITY---------------
    template<int...> struct index_tuple{};

    template<int I, typename IndexTuple, typename... Types>
    struct make_indexes_impl;

    template<int I, int... Indexes, typename T, typename ... Types>
    struct make_indexes_impl<I, index_tuple<Indexes...>, T, Types...>
    {
        typedef typename make_indexes_impl<I + 1, index_tuple<Indexes..., I>, Types...>::type type;
    };

    template<int I, int... Indexes>
    struct make_indexes_impl<I, index_tuple<Indexes...> >
    {
        typedef index_tuple<Indexes...> type;
    };

    template<typename ... Types>
    struct make_indexes : make_indexes_impl<0, index_tuple<>, Types...>{};

    // ----------UNPACK TUPLE AND APPLY TO FUNCTION ---------

    template<class Ret, class... Args, int... Indexes>
    inline Ret exec__( Ret (*pf)(SocketSerialized&,Args...),SocketSerialized& sock, index_tuple< Indexes... >, std::tuple<Args...>&& args)
    {
        int ctx[] = {((sock>>std::get<Indexes>(args)), void(), 0)... };
        (void)ctx;
        sock.clear();
        Ret res = pf(sock,std::forward<Args>(std::get<Indexes>(args))...);
        sock<<res;
        sock.sendCl();
        return res;
    };

    template<typename Ret,typename ... Args>
    Ret FuncWrapper::exec(Ret(*pf)(SocketSerialized&,Args ...),SocketSerialized& sock)
    {
        return exec__(pf,sock,typename make_indexes<Args...>::type(), std::tuple<typename std::remove_reference<Args>::type...>());
    }
#endif
}

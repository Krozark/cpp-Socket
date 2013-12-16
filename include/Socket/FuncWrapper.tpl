#include <tuple>
#include <type_traits>

namespace ntw
{

    /***************************
     ********** CLIENT**********
     **************************/
    template<typename ... Args>
    void FuncWrapper::cli::addPackage(int id,SocketSerialized& sock,Args& ... args)
    {
        sock<<id;
        addPackage(sock,args...);
    }


    template<typename T,typename ... Args>
    void FuncWrapper::cli::addPackage(SocketSerialized& sock,T& a,Args& ... args)
    {
        sock<<a;
        addPackage(sock,args...);
    }

    template<typename Ret,typename ... Args>
    Ret FuncWrapper::cli::send(SocketSerialized& sock,Status& st,int id,Args&& ... args)
    {
        Ret ret;
        addPackage(id,sock,args ...);
        sock.send();
        if (sock.receive() > 0)
        {
            sock>>st;
            if(st.code != ntw::FuncWrapper::Status::st::wrong_id)
            {
                sock>>ret;
            }
            else
            {
                std::cerr<<"Recive Status different \"ok\""<<std::endl;
            }
        }
        return ret;
    }


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
    inline Ret exec__( Ret (*pf)(SocketSerialized&,int&,Args...),SocketSerialized& sock, index_tuple< Indexes... >, std::tuple<Args...>&& args)
    {
        int ctx[] = {((sock>>std::get<Indexes>(args)), void(), 0)... };
        (void)ctx;
        sock.clear();
        int status = ntw::FuncWrapper::Status::ok;
        Ret res = pf(sock,status,std::forward<Args>(std::get<Indexes>(args))...);
        sock<<ntw::FuncWrapper::Status(status)<<res;
        sock.sendCl();
        return res;
    };

    /***********************************
     * ******** SERVER ****************
     * *********************************/
    template<typename Ret,typename ... Args>
    Ret FuncWrapper::srv::exec(Ret(*pf)(SocketSerialized&,int&,Args ...),SocketSerialized& sock)
    {
        return exec__(pf,sock,typename make_indexes<Args...>::type(), std::tuple<typename std::remove_reference<Args>::type...>());
    }
}

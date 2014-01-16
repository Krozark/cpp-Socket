#include <Socket/SelectManager.hpp>

#include <iostream>
#include <algorithm>

#include <errno.h>
#include <string.h>
 	
namespace ntw {

template<class C,typename ... Args>
void thread_method(C* obj,void(C::*func)(Args ...),Args ... args)
{
    (obj->*func)(args ...);
};

SelectManager::SelectManager(float t): 
    onSelect(0),
    data(0),
    do_delete(false),
    readfds(0), 
    writefds(0),
    exceptfds(0),
    _run(false)
{
    setTimout(t);
};

SelectManager::~SelectManager()
{
    clear();
}

void SelectManager::add(SocketSerialized* s)
{
    mutex.lock();
    datas.emplace_back(std::make_tuple(s,std::thread(),new bool(_run)));
    
    if(_run)
    {
        auto& tuple = datas.back();
        SocketSerialized* sock = std::get<0>(tuple);
        bool* b = std::get<2>(tuple);

        std::get<1>(tuple) = std::thread(&SelectManager::run,this,sock,b);
    }

    mutex.unlock();
};

bool SelectManager::remove(SocketSerialized* s)
{
    mutex.lock();
    auto end = datas.end();
    for(auto it = datas.begin();it!= end;++it)
    {
        if(s == std::get<0>(*it))
        {
            *std::get<2>(*it) = false; //run = false
            std::get<1>(*it).detach();
            datas.erase(it);//memory free at the end of thread execution (in run)
            mutex.unlock();
            return true;
        }
    }
    mutex.unlock();
    return false;
};
void SelectManager::clear()
{
    if(do_delete)
    {
        for(auto& tuple : datas)
        {
            delete std::get<0>(tuple);//soket
            delete std::get<2>(tuple);//bool*
        }
    }
    else //delete the bool
    {
        for(auto& tuple : datas)
        {
            delete std::get<2>(tuple);//bool*
        }
    }

    datas.clear();

};

void SelectManager::setArgs(bool read,bool write,bool except,float timeout_sec)
{
    setRead(read);
    setWrite(write);
    setExcept(except);
    setTimout(timeout_sec);
};

void SelectManager::setRead(bool read)
{
    mutex.lock();
    readfds = read;
    mutex.unlock();
};

void SelectManager::setWrite(bool write)
{
    mutex.lock();
    writefds = write;
    mutex.unlock();
};

void SelectManager::setExcept(bool except)
{
    mutex.lock();
    exceptfds = except;
    mutex.unlock();
};

void SelectManager::setTimout(float timeout_sec)
{
    mutex.lock();
    timeout.tv_sec = (int)timeout_sec;
    timeout.tv_usec = (int)timeout_sec*100000;//10⁻⁶
    mutex.unlock();
};


void SelectManager::start()
{
    thread = std::thread(&SelectManager::run_me,this);
}

void SelectManager::run_me()
{
    mutex.lock();
    _run = true;
    for(auto& tuple : datas)
    {
        SocketSerialized* sock = std::get<0>(tuple);
        bool* b = std::get<2>(tuple);
        *b = true;

        std::get<1>(tuple) = std::thread(&SelectManager::run,this,sock,b);
    }
    mutex.unlock();

    while(_run)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
};

void SelectManager::stop()
{
    mutex.lock();
    _run=false;
    for(auto& tuple : datas)
    {
        bool* b = std::get<2>(tuple);
        *b = false;
    }
    mutex.unlock();
}

void SelectManager::wait()
{
    thread.join();
    /*for(auto& tuple : datas)
        std::get<1>(tuple).join();*/
}

void SelectManager::detach()
{
    thread.detach();
    for(auto& tuple : datas)
        std::get<1>(tuple).detach();
}

unsigned int SelectManager::size()const
{
    return datas.size();
}

void SelectManager::setDelete(bool d)
{
    do_delete = d;
}

void SelectManager::run(SocketSerialized* sock_ptr,bool* run_ptr)
{
    SocketSerialized& sock = *sock_ptr;
    bool& run = *run_ptr;

    int res;
    fd_set* read = readfds?new fd_set:0; ///< flag for select
    fd_set* write = writefds?new fd_set:0; ///< flag for select
    fd_set* except = exceptfds?new fd_set:0; ///< flag for select

    const int id = sock.id();
    const int max_id = id + 1;

    while(run)
    {
        if(read)
        {
            FD_ZERO(read);
            FD_SET(id,read);
        }
        if(write)
        {
            FD_ZERO(write);
            FD_SET(id,write);
        }
        if(except)
        {
            FD_ZERO(except);
            FD_SET(id,except);
        }
        
        timeval time = timeout;//copy
        res = select(max_id,read,write,except,&time);

        if(res <0)
        {
            perror("select()");
            return;
        }
        else if (res == 0) //timeout
            continue;

        onSelect(*this,data,sock);
    }


    if(read)
    {
        delete read;
    }
    if(write)
    {
        delete write;
    }
    if(except)
    {
        delete except;
    }

    delete run_ptr;
    if(do_delete)
        delete sock_ptr;
};

};

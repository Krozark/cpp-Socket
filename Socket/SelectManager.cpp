#include <Socket/SelectManager.hpp>

#include <iostream>
#include <algorithm>

#include <errno.h>
#include <string.h>
 	
#ifdef _WIN32 //_WIN64
#include <fcntl.h>
#include <io.h>
#endif

namespace ntw {

SelectManager::SelectManager(float t): readfds(0), writefds(0), exceptfds(0), onSelect(0), max_id(0), _run(false)
{
    setTimout(t);
    #ifdef _WIN32 //_WIN64
    ::_pipe(pipe_fd,16,O_BINARY);
    #elif __linux
    ::pipe(pipe_fd);//pipe_fd[0] = read, [1] = write
    #else
    #error pipe not defined for this platform
    #endif
};

SelectManager::~SelectManager()
{
    #ifdef _WIN32 //_WIN64
    ::_close(pipe_fd[0]);
    ::_close(pipe_fd[1]);
    #elif __linux
    ::close(pipe_fd[0]);
    ::close(pipe_fd[1]);
    #else
    #error pipe not defined for this platform
    #endif

    clear();

    if(readfds)
        delete readfds;
    if(writefds)
        delete writefds;
    if(exceptfds)
        delete exceptfds;
}

void SelectManager::add(SocketSerialized* s)
{
    int id = s->id();
    datas.emplace_back(s);
    breakSelect();
};

bool SelectManager::remove(SocketSerialized* s)
{
    int id = s->id();
    auto end = datas.end();
    auto it = std::find(datas.begin(),end,s);
    if(it != end)
    {
        datas.erase(it);
        breakSelect();
        return true;
    }
    return false;
};
void SelectManager::clear()
{
    for(Socket* s : datas)
        delete s;

    datas.clear();
    breakSelect();

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
    if(read)
    {
        if(not readfds)
            readfds = new fd_set;
    }
    else
    {
        if(readfds)
        {
            delete readfds;
            readfds = 0;
        }
    }
    mutex.unlock();
};

void SelectManager::setWrite(bool write)
{
    mutex.lock();
    if(write)
    {
        if(not writefds)
            writefds = new fd_set;
    }
    else
    {
        if(writefds)
        {
            delete writefds;
            writefds = 0;
        }
    }
    mutex.unlock();
};

void SelectManager::setExcept(bool except)
{
    mutex.lock();
    if(except)
    {
        if(not exceptfds)
            exceptfds = new fd_set;
    }
    else
    {
        if(exceptfds)
        {
            delete exceptfds;
            exceptfds = 0;
        }
    }
    mutex.unlock();
};

void SelectManager::setTimout(float timeout_sec)
{
    mutex.lock();
    timeout.tv_sec = (int)timeout_sec;
    timeout.tv_usec = (int)timeout_sec*100000;//10⁻⁶
    mutex.unlock();
};

template<class C,typename ... Args>
void thread_method(C* obj,void(C::*func)(Args ...),Args ... args)
{
    (obj->*func)(args ...);
};

void SelectManager::start()
{
    mutex.lock();
    _run = true;
    thread= std::thread(thread_method<SelectManager>,this,&SelectManager::run);
    mutex.unlock();
};

void SelectManager::stop()
{
    mutex.lock();
    _run=false;
    breakSelect();
    mutex.unlock();
}

void SelectManager::run()
{
    int res;
    while(_run)
    {
        reset();//TODO
        
        auto time = timeout;//copy
        res = select(max_id,readfds,writefds,exceptfds,&time);

        if(res <0)
        {
            perror("select()");
            return;
        }
        else if (res == 0) //timout
            continue;
        else
        {
            if( (readfds and FD_ISSET(pipe_fd[0],readfds))
                or (writefds and FD_ISSET(pipe_fd[0],writefds))
                or (exceptfds and FD_ISSET(pipe_fd[0],exceptfds)))
            {
                char buffer[16];
                #ifdef _WIN32 //_WIN64
                ::_read(pipe_fd[0], buffer, sizeof(buffer));
                #elif __linux
                ::read(pipe_fd[0], buffer, sizeof(buffer));
                #else
                #error pipe not defined for this platform
                #endif
                continue;
            }
        }

        //loop sur les Socket pour savoir si c'est elle
        auto end = datas.end();
        for(auto it=datas.begin();it!=end /*and res > 0*/;++it)
        {
            auto& iit = **it;
            int id = iit.id(); 
            if(readfds and FD_ISSET(id,readfds))
            {
                onSelect(*this,iit);
                --res;
                continue;
            }
            if(writefds and FD_ISSET(id,writefds))
            {
                onSelect(*this,iit);
                --res;
                continue;
            }
            if(exceptfds and FD_ISSET(id,exceptfds))
            {
                onSelect(*this,iit);
                --res;
                continue;
            }
        }
    }
};

void SelectManager::reset()
{
    //reset
    if(readfds)
        FD_ZERO(readfds);
    if(writefds)
        FD_ZERO(writefds);
    if(exceptfds)
        FD_ZERO(exceptfds);

    max_id = pipe_fd[0]+1;
    //pipe add
    if(readfds)
        FD_SET(pipe_fd[0],readfds);
    if(writefds)
        FD_SET(pipe_fd[0],writefds);
    if(exceptfds)
        FD_SET(pipe_fd[0],exceptfds);

    auto end = datas.end();
    // add to the connection all socket
    for(auto it=datas.begin();it!=end;++it)
    {
        int id = (*it)->id();
        if(id>=max_id)
            max_id=id+1;
        //add socket
        if(readfds)
            FD_SET(id,readfds);
        if(writefds)
            FD_SET(id,writefds);
        if(exceptfds)
            FD_SET(id,exceptfds);
    }
};

void SelectManager::breakSelect()
{
    char buffer = 1;
    #ifdef _WIN32 //_WIN64
    ::_write(pipe_fd[1],&buffer,1); //juste pour break le select
    #elif __linux
    ::write(pipe_fd[1],&buffer,1); //juste pour break le select
    #else
    #error pipe not defined for this platform
    #endif
}




};

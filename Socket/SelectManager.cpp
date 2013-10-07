#include "SelectManager.hpp"

#include <iostream>
#include <algorithm>

#include <errno.h>
#include <signal.h>
#include <string.h>

namespace ntw {

SelectManager::SelectManager(): readfds(0), writefds(0), exceptfds(0), onSelect(0), max_id(0), _run(false)
{
    setTimout(5);
};

SelectManager::~SelectManager()
{
    if(readfds)
        delete readfds;
    if(writefds)
        delete writefds;
    if(exceptfds)
        delete exceptfds;
}

void SelectManager::add(Socket* s)
{
    int id = s->id();
    datas.emplace_back(s);
    max_id=(id>max_id)?id+1:max_id;
    if(readfds)
        FD_SET(id,readfds);
    if(writefds)
        FD_SET(id,writefds);
    if(exceptfds)
        FD_SET(id,exceptfds);
};

void SelectManager::remove(Socket* s)
{
    int id = s->id();
    auto end = datas.end();
    auto it = std::find(datas.begin(),end,s);
    if(it != end)
        datas.erase(it);
    reset();
};
void SelectManager::clear()
{
    datas.clear();
    max_id=0;
    if(readfds)
        FD_ZERO(readfds);
    if(writefds)
        FD_ZERO(writefds);
    if(exceptfds)
        FD_ZERO(exceptfds);
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

    auto end = datas.end();
    max_id = 0;
    // add to the connection all socket
    for(auto it=datas.begin();it!=end;++it)
    {
        int id = (*it)->id();
        max_id=(id>=max_id)?id+1:max_id;
        //add socket
        if(readfds)
            FD_SET(id,readfds);
        if(writefds)
            FD_SET(id,writefds);
        if(exceptfds)
            FD_SET(id,exceptfds);
    }
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

// Signal handler to catch SIGTERM.

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

};

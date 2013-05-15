#include "SelectManager.hpp"

#include <iostream>
#include <algorithm>

#include <errno.h>
#include <signal.h>
#include <string.h>

namespace ntw {

SelectManager::SelectManager(): readfds(0), writefds(0), exceptfds(0), OnSelect(0), max_id(0), run(false)
{
    SetTimout(5);
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

void SelectManager::Add(Socket* s)
{
    int id = s->Id();
    datas.emplace_back(s);
    max_id=(id>max_id)?id+1:max_id;
    if(readfds)
        FD_SET(id,readfds);
    if(writefds)
        FD_SET(id,writefds);
    if(exceptfds)
        FD_SET(id,exceptfds);
};

void SelectManager::Remove(Socket* s)
{
    int id = s->Id();
    auto end = datas.end();
    auto it = std::find(datas.begin(),end,s);
    if(it != end)
        datas.erase(it);
    Reset();
};
void SelectManager::Clear()
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

void SelectManager::Reset()
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
        int id = (*it)->Id();
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

void SelectManager::SetArgs(bool read,bool write,bool except,float timeout_sec)
{
    SetRead(read);
    SetWrite(write);
    SetExcept(except);
    SetTimout(timeout_sec);
};

void SelectManager::SetRead(bool read)
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

void SelectManager::SetWrite(bool write)
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

void SelectManager::SetExcept(bool except)
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

void SelectManager::SetTimout(float timeout_sec)
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

void SelectManager::Start()
{
    mutex.lock();
    run = true;
    thread= std::thread(thread_method<SelectManager>,this,&SelectManager::Run);
    mutex.unlock();
};

// Signal handler to catch SIGTERM.

void SelectManager::Run()
{
    int res;
    while(run)
    {
        Reset();//TODO
        
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
            int id = iit.Id(); 
            if(readfds and FD_ISSET(id,readfds))
            {
                OnSelect(*this,iit);
                --res;
                continue;
            }
            if(writefds and FD_ISSET(id,writefds))
            {
                OnSelect(*this,iit);
                --res;
                continue;
            }
            if(exceptfds and FD_ISSET(id,exceptfds))
            {
                OnSelect(*this,iit);
                --res;
                continue;
            }
        }
    }
};

};

#include "SelectManager.hpp"

#include <iostream>




namespace ntw {

SelectManager::SelectManager(): readfds(0), writefds(0), exceptfds(0), timeout(0), OnSelect(0), max_id(0), run(false)
{
};

SelectManager::~SelectManager()
{
    if(readfds)
        delete readfds;
    if(writefds)
        delete writefds;
    if(exceptfds)
        delete exceptfds;
    if(timeout)
        delete timeout;
}

void SelectManager::Remove(Socket* s)
{
    auto it = datas.find(s->Id());
    if(it != datas.end())
        datas.erase(it);
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
};

void SelectManager::SetWrite(bool write)
{
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
};

void SelectManager::SetExcept(bool except)
{
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
};

void SelectManager::SetTimout(float timeout_sec)
{
    if(timeout_sec > 0.f)
    {
        if(not timeout)
            timeout = new timeval;
        timeout->tv_sec = (int)timeout_sec;
        timeout->tv_usec = (int)timeout_sec*100000;//10⁻⁶
    }
    else
    {
        if(timeout)
        {
            delete timeout;
            timeout = 0;
        }
    }
};

template<class C,typename ... Args>
void thread_method(C* obj,void(C::*func)(Args ...),Args ... args)
{
    (obj->*func)(args ...);
};

void SelectManager::Start()
{
    thread= std::thread(thread_method<SelectManager>,this,&SelectManager::Run);
};

void SelectManager::Run()
{
    run = true;

    if(readfds)
        FD_ZERO(readfds);
    if(writefds)
        FD_ZERO(writefds);
    if(exceptfds)
        FD_ZERO(exceptfds);
    /* add the connection socket */
    auto end = datas.end();
    for(auto it=datas.begin();it!=end;++it)
    {
        int id = it->second->Id();
        if(readfds)
            FD_SET(id,readfds);
        if(writefds)
            FD_SET(id,writefds);
        if(exceptfds)
            FD_SET(id,exceptfds);
    }
    

    while(run)
    {
        int res;
        if(timeout)
        {
            timeval time = *timeout;
            res = select(max_id,readfds,writefds,exceptfds,&time);
        }
        else
        {
            res = select(max_id,readfds,writefds,exceptfds,NULL);
        }


        if(res <0)
        {
            perror("select()");
            return;
        }

        //loop sur les Socket pour savoir si c'est elle
        auto end = datas.end();
        for(auto it=datas.begin();it!=end and res > 0;++it)
        {
            auto& iit = *it;
            if(readfds and FD_ISSET(iit.first,readfds))
            {
                OnSelect(*this,*(iit.second));
                --res;
                continue;
            }
            if(writefds and FD_ISSET(iit.first,writefds))
            {
                OnSelect(*this,*(iit.second));
                --res;
                continue;
            }
            if(exceptfds and FD_ISSET(iit.first,exceptfds))
            {
                OnSelect(*this,*(iit.second));
                --res;
                continue;
            }
        }
    }
};

};

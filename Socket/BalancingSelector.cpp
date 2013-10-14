#include <Socket/BalancingSelector.hpp>
#include <iostream>

namespace ntw 
{
    BalancingSelector::BalancingSelector(bool read, bool write, bool except,void (*onSel)(SelectManager&,Socket&), unsigned int _min,unsigned int _max,unsigned int max_selector,float timeout):  readfds(read),
    writefds(write),
    exceptfds(except),
    onSelect(onSel),
    nb_selector_max(max_selector),
    max_per_selector(_max),
    min_per_selector(_min),
    _run(false),
    timeout(timeout)
    {
        SelectManager& s = newSelector();
        s.setArgs(readfds,writefds,exceptfds,timeout);
        s.onSelect = onSelect;
    }

    bool BalancingSelector::add(Socket* s)
    {
        unsigned int min_charge = -1;
        SelectManager* min_selector;

        for(SelectManager& selector : selectors)
        {
            unsigned int s = selectors.size();
            if(s <min_charge)
            {
                min_charge = s;
                min_selector = &selector;
            } 
        }

        if(min_charge < max_per_selector)
        {
            min_selector->add(s);
        }
        else
        {
            if(nb_selector_max == 0 or selectors.size() < nb_selector_max)
            {
                SelectManager& selector = newSelector();
                selector.add(s);
            }
            return false;
        }
        return true;
    }

    void BalancingSelector::remove(Socket* s)
    {
        int i = 0; //pour garder au moins 1 selector
        for(SelectManager& selector : selectors)
        {
            if(selector.remove(s))
            {
                if(i>0 and selector.size() < min_per_selector)
                {
                    //\todo
                    std::cerr<<"[TODO] killer le selector, et redistribuer les sockets"<<std::endl;
                }

            }
            ++i;
        }
    }

    void BalancingSelector::clear()
    {
        for(SelectManager& selector : selectors)
        {
            selector.clear();
        }
        selectors.clear();
    }

    void BalancingSelector::start()
    {
        _run = true;
        for(SelectManager& selector : selectors)
            selector.start();
    }

    void BalancingSelector::stop()
    {
        _run = false;
        for(SelectManager& selector : selectors)
            selector.stop();
    }

    void BalancingSelector::wait()
    {
        for(SelectManager& selector : selectors)
            selector.wait();
    }

    /*void BalancingSelector::detach()
    {
        for(SelectManager& selector : selectors)
            selector.detach();
    }*/


    unsigned int BalancingSelector::size()const
    {
        unsigned int size = 0;
        for(const SelectManager& selector : selectors)
            size += selectors.size();
        return size;
    }

    void BalancingSelector::setArgs(bool read,bool write,bool except,float timeout_sec)
    {
        writefds = write;
        readfds = read;
        exceptfds = except;
        timeout = timeout_sec;
        for(SelectManager& selector : selectors)
            selector.setArgs(read,write,except,timeout_sec);
    }

    void BalancingSelector::setRead(bool read)
    {
        readfds = read;
        for(SelectManager& selector : selectors)
            selector.setRead(read);
    }

    void BalancingSelector::setWrite(bool write)
    {
        writefds = write;
        for(SelectManager& selector : selectors)
            selector.setWrite(write);
    }

    void BalancingSelector::setExcept(bool except)
    {
        exceptfds = except;
        for(SelectManager& selector : selectors)
            selector.setExcept(except);
    }

    void BalancingSelector::setTimout(float timeout_sec)
    {
        timeout = timeout_sec;
        for(SelectManager& selector : selectors)
            selector.setTimout(timeout_sec);
    }

    SelectManager& BalancingSelector::newSelector()
    {
        selectors.emplace_back();

        SelectManager& s = selectors.back();
        s.onSelect = onSelect;
        s.setArgs(readfds,writefds,exceptfds,timeout);

        if(_run)
            s.start();
        /*if(selectors.size()>1)
            s.detach(); ///< todo
        */
        return s;
    }
}

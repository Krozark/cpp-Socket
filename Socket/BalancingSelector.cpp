#include <Socket/BalancingSelector.hpp>
#include <iostream>

namespace ntw 
{
    BalancingSelector::BalancingSelector(unsigned int _min,unsigned int _max,unsigned int max_selector,float timeout): nb_selector_max(max_selector), max_per_selector(_max), min_per_selector(_min), _run(false)
    {
        newSelector().setTimout(timeout);

    }

    void BalancingSelector::add(Socket* s)
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
            SelectManager& selector = newSelector();
            selector.add(s);
        }
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

    unsigned int BalancingSelector::size()const
    {
        unsigned int size = 0;
        for(const SelectManager& selector : selectors)
            size += selectors.size();
        return size;
    }

    SelectManager& BalancingSelector::newSelector()
    {

        selectors.emplace_back();

        SelectManager& s = selectors.back();
        s.onSelect = onSelect;

        if(selectors.size()>0)
        {
            const SelectManager& front = selectors.front();
            if(front.readfds)
                s.setRead(true);
            if(front.writefds)
                s.setWrite(true);
            if(front.exceptfds)
                s.setExcept(false);
            s.timeout = front.timeout;
        }

        if(_run)
            s.start();
    }
}

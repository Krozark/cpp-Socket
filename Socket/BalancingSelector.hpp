#ifndef NTW_BALANCINGSELECTOR_HPP
#define NTW_BALANCINGSELECTOR_HPP

#include <Socket/SelectManager.hpp>

#include <list>

namespace ntw
{
    class BalancingSelector
    {
        explicit BalancingSelector(unsigned int min_per_selector=50,unsigned int max_per_selector=100,unsigned int nb_selector_max=-1,float timeout=NTW_DEFAULT_TIMEOUT);

        BalancingSelector(const BalancingSelector&) = delete;
        BalancingSelector& operator=(const BalancingSelector&) = delete;

        void add(Socket* s);
        void remove(Socket* s);
        void clear();
        void start();
        void stop();
        unsigned int size()const;
        
        void setArgs(bool read=false,bool write=false,bool except=false,float timeout_sec=NTW_DEFAULT_TIMEOUT);
        void setRead(bool read=false);
        void setWrite(bool write=false);
        void setExcept(bool except=false);
        void setTimout(float timeout_sec=NTW_DEFAULT_TIMEOUT);
        
        void(*onSelect)(SelectManager& self,Socket& s);


        private:
            const unsigned int nb_selector_max;
            const unsigned int max_per_selector;
            const unsigned int min_per_selector;

            std::list<SelectManager> selectors;

            SelectManager& newSelector();
            bool _run;
    };
}

#endif

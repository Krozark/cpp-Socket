#ifndef NTW_BALANCINGSELECTOR_HPP
#define NTW_BALANCINGSELECTOR_HPP

#include <Socket/SelectManager.hpp>

#include <list>

namespace ntw
{
    class BalancingSelector
    {
        public:
            BalancingSelector(bool read, bool write, bool except,void (*onSelect)(SelectManager&,Socket&),unsigned int min_per_selector=50,unsigned int max_per_selector=100,unsigned int nb_selector_max=0,float timeout=NTW_DEFAULT_TIMEOUT);

            BalancingSelector(const BalancingSelector&) = delete;
            BalancingSelector& operator=(const BalancingSelector&) = delete;

            bool add(Socket* s);
            bool add(Socket::Dommaine dommaine,Socket::Type type,std::string host,int port=NTW_PORT);
            void remove(Socket* s);
            void clear();
            void start();
            void stop();
            void wait();
            //void detach();
            unsigned int size()const;
            
            void setArgs(bool read=false,bool write=false,bool except=false,float timeout_sec=NTW_DEFAULT_TIMEOUT);
            void setRead(bool read=false);
            void setWrite(bool write=false);
            void setExcept(bool except=false);
            void setTimout(float timeout_sec=NTW_DEFAULT_TIMEOUT);
        


        private:
            const unsigned int nb_selector_max;
            const unsigned int max_per_selector;
            const unsigned int min_per_selector;

            float timeout;
            bool  readfds;
            bool  writefds;
            bool  exceptfds;
            void(*onSelect)(SelectManager& self,Socket& s);

            std::list<SelectManager> selectors;

            SelectManager& newSelector();
            bool _run;
    };
}

#endif

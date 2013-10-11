#ifndef NTW_SELECTMANAGER_HPP
#define NTW_SELECTMANAGER_HPP

#include <vector>
#include <thread>
#include <mutex>

#include <Socket/define.hpp>
#include <Socket/Socket.hpp>

namespace ntw {

    class BalancingSelector;

    class SelectManager
    {

        public:
            explicit SelectManager(float timeout=NTW_DEFAULT_TIMEOUT);
            ~SelectManager();
            
            void add(Socket* s);
            bool remove(Socket* s);
            void clear();

            void(*onSelect)(SelectManager& self,Socket& s);
            void setArgs(bool read=false,bool write=false,bool except=false,float timeout_sec=NTW_DEFAULT_TIMEOUT);
            void setRead(bool read=false);
            void setWrite(bool write=false);
            void setExcept(bool except=false);
            void setTimout(float timout_sec=NTW_DEFAULT_TIMEOUT);

            void start(); //create a thread and lunch Run() a loop while(run); ie while Stop() is not called
            void stop();
            inline void wait(){thread.join();};
            inline void detach(){thread.detach();};

            inline unsigned int size()const{return datas.size();};

            SelectManager(const SelectManager& other) = delete;
            SelectManager& operator=(const SelectManager& other) = delete;

        private:
            friend class BalancingSelector;
            void run(); //Use Start to run it
            void reset();
            void breakSelect();

            fd_set* readfds;
            fd_set* writefds;
            fd_set* exceptfds;
            timeval timeout;
            std::vector<Socket*> datas;
            volatile int max_id;
            volatile bool _run;
            std::thread thread;
            std::mutex mutex;

            int pipe_fd[2];
    };

};

#endif

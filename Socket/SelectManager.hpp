#ifndef NTW_SELECTMANAGER_HPP
#define NTW_SELECTMANAGER_HPP

#include <vector>
#include <thread>
#include <mutex>

#include <Socket/define.hpp>
#include <Socket/Socket.hpp>

namespace ntw {

class SelectManager
{
    public:
        explicit SelectManager(float timeout=NTW_DEFAULT_TIMEOUT);
        ~SelectManager();
        
        void add(Socket* s);
        void remove(Socket* s);
        void clear();

        void(*onSelect)(SelectManager& self,Socket& s);
        void setArgs(bool read=false,bool write=false,bool except=false,float timeout_sec=0);
        void setRead(bool read=false);
        void setWrite(bool write=false);
        void setExcept(bool except=false);
        void setTimout(float timout_sec=0);

        void start(); //create a thread and lunch Run() a loop while(run); ie while Stop() is not called
        inline void stop(){
            mutex.lock();
            _run=false;
            mutex.unlock();
        };
        inline void wait(){thread.join();};
        inline void detach(){thread.detach();};

        SelectManager(const SelectManager& other) = delete;
        SelectManager& operator=(const SelectManager& other) = delete;

    private:
        void run(); //Use Start to run it
        void reset();

        fd_set* readfds;
        fd_set* writefds;
        fd_set* exceptfds;
        timeval timeout;
        int pipe_fd[2];
        std::vector<Socket*> datas;
        volatile int max_id;
        volatile bool _run;
        std::thread thread;
        std::mutex mutex;
};

};

#endif

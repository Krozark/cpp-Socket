#ifndef SELECTMANAGER_HPP
#define SELECTMANAGER_HPP

#include <unordered_map>
#include <thread>

#include "Socket.hpp"

namespace ntw {

class SelectManager
{
    public:
        explicit SelectManager();
        ~SelectManager();
        
        inline void Add(Socket* s){
            int id = s->Id();
            datas[id]=s;
            max_id=(id>max_id)?id+1:max_id;
        };
        void Remove(Socket* s);
        inline void Clear(){datas.clear();max_id=0;};

        void(*OnSelect)(SelectManager& self,Socket& s);
        void SetArgs(bool read=false,bool write=false,bool except=false,float timeout_sec=0);
        void SetRead(bool read=false);
        void SetWrite(bool write=false);
        void SetExcept(bool except=false);
        void SetTimout(float timout_sec=0);

        void Start(); //create a thread and lunch Run() a loop while(run); ie while Stop() is not called
        inline void Stop(){run=false;thread.join();};
        void Run();

        SelectManager(const SelectManager& other) = delete;
        SelectManager& operator=(const SelectManager& other) = delete;
    private:

        fd_set* readfds;
        fd_set* writefds;
        fd_set* exceptfds;
        timeval* timeout;
        std::unordered_map<int,Socket*> datas;
        int max_id;
        bool run;
        std::thread thread;
};

};

#endif

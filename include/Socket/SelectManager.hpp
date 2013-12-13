#ifndef NTW_SELECTMANAGER_HPP
#define NTW_SELECTMANAGER_HPP

#include <vector>
#include <thread>
#include <mutex>

#include <Socket/define.hpp>
#include <Socket/SocketSerialized.hpp>
#include <Socket/Config.hpp>

namespace ntw {

    class BalancingSelector;

    class SelectManager
    {

        public:
            explicit SelectManager(float timeout=Config::default_timeout);
            ~SelectManager();
            
            void add(SocketSerialized* s);
            bool remove(SocketSerialized* s);
            void clear();

            void(*onSelect)(SelectManager& self,void* data,SocketSerialized& s);
            void* data;

            void setArgs(bool read=false,bool write=false,bool except=false,float timeout_sec=Config::default_timeout);
            void setRead(bool read=false);
            void setWrite(bool write=false);
            void setExcept(bool except=false);
            void setTimout(float timout_sec=Config::default_timeout);

            void start(); //create a thread and lunch Run() a loop while(run); ie while Stop() is not called
            void stop();
            inline void wait(){thread.join();};
            inline void detach(){thread.detach();};
            //inline bool joinable(){return thread.joinable();};

            inline unsigned int size()const{return datas.size();};
            inline void setDelete(bool d){do_delete = d;};

            SelectManager(const SelectManager& other) = delete;
            SelectManager& operator=(const SelectManager& other) = delete;

        private:
            friend class BalancingSelector;
            void run(); //Use Start to run it
            void reset();
            void breakSelect();
            bool do_delete;

            fd_set* readfds;
            fd_set* writefds;
            fd_set* exceptfds;
            timeval timeout;
            std::vector<SocketSerialized*> datas;
            volatile int max_id;
            volatile bool _run;
            std::thread thread;
            std::mutex mutex;

            int pipe_fd[2];
    };

};

#endif

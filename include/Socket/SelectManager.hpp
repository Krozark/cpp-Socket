#ifndef NTW_SELECTMANAGER_HPP
#define NTW_SELECTMANAGER_HPP

#include <list>
#include <thread>
#include <mutex>
#include <tuple>

#include <Socket/define.hpp>
#include <Socket/SocketSerialized.hpp>
#include <Socket/Config.hpp>

namespace ntw {

    class BalancingSelector;

    /**
     * \brief Magane multiple soket in a thread and call select C function on them
     */
    class SelectManager
    {

        public:
            /**
             * \brief Constructor
             * \param timeout the time for the select function
             */
            explicit SelectManager(float timeout=Config::default_timeout);

            /**
             * \brief Destructor
             */
            ~SelectManager();
            
            /**
             * \brief Add a socket to manage
             * \param s the soket to manage
             */
            void add(SocketSerialized* s);

            /**
             * \brief remove a socket of the manager
             * \param s The socket to remove
             */
            bool remove(SocketSerialized* s);

            /**
             * \brief Remove all the sokets
             * If do_delete is true, delete all of them
             * need to be call after a wait() to avoid leak.
             */
            void clear();

            /**
             * \brief a function to call whene a socket as benn selected
             * \param self *this
             * \param data data. \see data
             * \param s The socket selected
             */
            void(*onSelect)(SelectManager& self,void* data,SocketSerialized& s);

            /**
             * \brief the data to send as onSelect param
             * default is 0
             */
            void* data;

            /**
             * \brief set the select mode
             * \param read call setRead with read as param
             * \param write call setWrite with write as param
             * \param except call setExcept with except as param
             * \param timout_sec call setTimout with timout_sec as param
             */
            void setArgs(bool read=false,bool write=false,bool except=false,float timeout_sec=Config::default_timeout);

            /**
             * \brief Set the Read mode for selection
             */
            void setRead(bool read=false);

            /**
             * \brief Set the Write mod for selection
             */
            void setWrite(bool write=false);

            /**
             * \brief Selt the except mode for selection
             */
            void setExcept(bool except=false);

            /**
             * \brief Set the maximum time during 2 selection
             */
            void setTimout(float timout_sec=Config::default_timeout);

            /**
             * \brief create a thread and lunch Run() a loop while(run); ie while Stop() is not called
             */
            void start(); 

            /**
             * \brief stop the run function
             */
            void stop();

            /**
             * \brief wait the end of the thread
             */
            void wait();

            /**
             * \brief Detache the internal thed execution
             */
            void detach();

            /**
             * \return the number of socket to manage
             */
            unsigned int size()const;

            /**
             * \brief if true, delete soket on destruction and when clear id call
             */
            void setDelete(bool d);

            SelectManager(const SelectManager& other) = delete;
            SelectManager& operator=(const SelectManager& other) = delete;

        private:
            friend class BalancingSelector;
            void run(SocketSerialized* sock,bool* run); //run a specifique client on his thread
            void run_me(); ///< Use Start to run it
            bool do_delete;///< setDelet value

            bool readfds; ///< flag for select
            bool writefds; ///< flag for select
            bool exceptfds; ///< flag for select
            timeval timeout; ///<  flag for select

            std::list<std::tuple<SocketSerialized*,std::thread,bool*>> datas; ///< internal socket to manage
            volatile bool _run; ///< is running?
            std::mutex mutex; ///< the mutex to use
            std::thread thread;
    };
};

#endif

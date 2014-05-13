#ifndef NTW_BALANCINGSELECTOR_HPP
#define NTW_BALANCINGSELECTOR_HPP

#include <Socket/SelectManager.hpp>
#include <Socket/FuncWrapper.hpp>

#include <list>

namespace ntw
{
    /**
     * \brief A class tha manage multiple SelectManager of a specifique size
     */
    class BalancingSelector
    {
        public:
            /**
             * \brief Constructor
             * \param read the read select condition flag
             * \param writefds the select write condition flag
             * \param execution the select except condition flag
             * \param onSelect the callBack
             * \param data param for the call back
             * \param min_per_selector the number of minimal socket for each internal selector
             * \param max_per_selector the number of maximum socket for each selector
             * \param nb_selector_max the maximun number of selectors
             * \param timeout the timeout select flag
             */
            BalancingSelector(bool read, bool write, bool except,void (*onSelect)(SelectManager&,void*,SocketSerialized&),void* data,unsigned int min_per_selector=50,unsigned int max_per_selector=100,unsigned int nb_selector_max=0,float timeout=5.f);

            BalancingSelector(const BalancingSelector&) = delete;
            BalancingSelector& operator=(const BalancingSelector&) = delete;

            /**
             * \brief add a socket to manage
             * \param s the soket to manage
             * \return true if added
             */
            bool add(SocketSerialized* sock);

            /**
             * \brief add a socjket to manage.
             * Connet the soket, and add it if not fail
             * \param sock the socket to manage
             * \param host the host to connect to
             * \param port the port to connect to
             * \return true if connecte an added
             */
            bool add(SocketSerialized* sock,std::string host,int port);

            /**
             * \brief create a socket, and add
             * \return true if ok
             */
            bool add(Socket::Domaine domaine,Socket::Type type,std::string host,int port);

            /**
             * \brief remove a socket py is adresse
             * \param the socket to remove
             */
            void remove(SocketSerialized* s);

            /**
             * \brief remove all the sokets
             */
            void clear();

            /**
             * \brief call start in all internal SelectManager
             */
            void start();

            /**
             * \brief call stop on all internal SelectManager
             */
            void stop();

            /**
             * \brief call wait on all internal SelectManager
             */
            void wait();
            //void detach();
            /**
             * \return the number of sockets
             */
            unsigned int size()const;
            
            /**
             * \brief set the select mode
             * \param read call setRead with read as param
             * \param write call setWrite with write as param
             * \param except call setExcept with except as param
             * \param timout_sec call setTimout with timout_sec as param
             */
            void setArgs(bool read=false,bool write=false,bool except=false,float timeout_sec=5.f);

            /**
             * \brief Set the maximum time during 2 selection
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
            void setTimout(float timeout_sec=5.f);

            /**
             * \brief if true, delete soket on destruction and when clear id call
             */
            void setDelete(bool d);
        


        private:
            const unsigned int nb_selector_max;///< maximal number of selector 
            const unsigned int max_per_selector;///< number of socket for each selector
            const unsigned int min_per_selector;///< number of minimal socket for each selector

            float timeout; ///< timeout select flag
            bool  readfds;///< select flag
            bool  writefds; ///< select flag
            bool  exceptfds; ///< select flag
            void(*onSelect)(SelectManager& self,void* data,SocketSerialized& s);///<callback
            void* data; ///< callback data

            bool do_delete; ///< delete soket on clear()?

            std::list<SelectManager> selectors; ///< internal selectors

            SelectManager& newSelector(); ///< add a nex selector and init it
            bool _run; ///< is running?
    };
}

#endif

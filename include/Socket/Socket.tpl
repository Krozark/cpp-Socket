namespace ntw
{
        template<typename T>
        int Socket::send(const T* data,const size_t size,const int flags=MSG_NOSIGNAL) const
        {
            int res;
            if(need_connect)
            {
                res = ::send(sock,(const char*)data,size,flags);
            }
            else
            {
                res = ::sendto(sock,(const char*)data,size,flags,(SOCKADDR*)(&sock_cfg),sizeof(sock_cfg));
            }

            if(res <0)
            {
                perror("Send()");
                throw SocketExeption("Sending message fail");
            }
            return res;
        }

        template<typename T>
        int Socket::send(const T* data,const size_t size,const int flags,const Socket& other) const
        {
            int res;
            if((res = ::sendto(sock,(const char*)data,size,flags,(SOCKADDR*)(&other.sock_cfg),sizeof(other.sock_cfg))) ==  SOCKET_ERROR)
            {
                perror("Send()");
                throw SocketExeption("Sending message fail");
            }
            return res;
        }

        template<typename T>
        inline int Socket::receive(T* buffer,const size_t size,const int flags=0) const
        {
            int res;
            if(need_connect)
            {
                res = ::recv(sock,(char*)buffer,size,flags);
            }
            else
            {
                SOCKADDR_IN from = {0}; ///< configuration struct
                socklen_t from_size=sizeof(from);
                res = ::recvfrom(sock,(char*)buffer,size,flags,(SOCKADDR*)(&from),&from_size);
            }
            return res;
        }

        template<typename T>
        inline int Socket::receive(T* buffer,const size_t size,const int flags,Socket& other) const
        {
            socklen_t slen=sizeof(other.sock_cfg);
            return ::recvfrom(sock,(char*)buffer,size,flags,(SOCKADDR*)(&other.sock_cfg),&slen);
        }
}

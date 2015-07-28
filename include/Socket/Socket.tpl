namespace ntw
{
    template<typename T>
    int Socket::send(const T* data,const size_t size,const int flags) const
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

    template <typename ... Args>
    int Socket::send(const Serializer& data,Args&& ... args) const
    {
        uint32_t size = data.size();
        char* buffer = new char[size + 4];

        {
            uint8_t *d = (uint8_t *)&size;

            #if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
            buffer[0] = d[0];
            buffer[1] = d[1];
            buffer[2] = d[2];
            buffer[3] = d[3];
            #elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
            buffer[3] = d[0];
            buffer[2] = d[1];
            buffer[1] = d[2];
            buffer[0] = d[3];
            #else
            #error "byte orden not suported (PDP endian)"
            #endif

        }

        memcpy(buffer + 4,data.data(),size);

        int res = 0;
        try
        {
            res = send(buffer,size + 4,std::forward<Args>(args)...);
        }
        catch(SocketExeption& e)
        {
            delete[] buffer;
            throw e;
        }
        delete[] buffer;
        return res;
    }

    template<typename T>
    int Socket::receive(T* buffer,const size_t size,const int flags) const
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
    int Socket::receive(T* buffer,const size_t size,const int flags,Socket& other) const
    {
        socklen_t slen=sizeof(other.sock_cfg);
        return ::recvfrom(sock,(char*)buffer,size,flags,(SOCKADDR*)(&other.sock_cfg),&slen);
    }

    template<typename ... Args>
    int Socket::receive(Serializer& data,Args&& ... args) const
    {
        char buffer[4];
        int res = 0;
        if(need_connect)
        {
            res = receive(buffer,4,std::forward<Args>(args)...);
        }
        else
        {
            res = receive(buffer,4,MSG_PEEK);
        }

        if (res > 0)
        {
            uint32_t size = 0;
            uint8_t *d = (uint8_t *)&size;

            #if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
            d[0] = buffer[0];
            d[1] = buffer[1];
            d[2] = buffer[2];
            d[3] = buffer[3];
            #elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
            d[0] = buffer[3];
            d[1] = buffer[2];
            d[2] = buffer[1];
            d[3] = buffer[0];
            #else
            #error "byte orden not suported (PDP endian)"
            #endif

            char* buffer2;

            try
            {
                if(need_connect)
                {
                    buffer2 = new char[size];
                    res+= receive(buffer2,size,std::forward<Args>(args)...);
                }
                else
                {
                    buffer2 = new char[size + 4];
                    res = receive(buffer2,size + 4,std::forward<Args>(args)...);

                }
            }
            catch (SocketExeption& e)
            {
                delete[] buffer2;
                throw e;
            }

            if(need_connect)
                data.write(buffer2,size);
            else
                data.write(buffer2 + 4,size);

            delete[] buffer2;
        }

        return res;
    }
}

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
        auto size = data.size();
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

            std::cout<<"send() size: "<<size<<std::endl;
            for(int i=0;i<4;++i)
                std::cout<<"send() buffer["<<i<<"]: "<<int(buffer[i])<<std::endl;

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

        std::cout<<"receive"<<std::endl;

        char buffer[4];
        int res = receive(buffer,4,std::forward<Args>(args)...);

        std::cout<<"receive: "<<res<<std::endl;
        if (res > 0)
        {
            uint32_t size;
            uint8_t *d = (uint8_t *)&size;

            #if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
            d[0] = buffer[0];
            d[1] = buffer[1];
            d[2] = buffer[2];
            d[3] = buffer[3];
            #elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
            d[0] = d[3];
            d[1] = d[2];
            d[2] = d[1];
            d[3] = d[0];
            #else
            #error "byte orden not suported (PDP endian)"
            #endif

            std::cout<<"receive: "<<size<<std::endl;
            for(int i=0;i<4;++i)
                std::cout<<"receive() buffer["<<i<<"]: "<<int(buffer[i])<<std::endl;

            char* buffer2 = new char[size];

            try
            {
                res+= receive(buffer2,size,std::forward<Args>(args)...);
            }
            catch (SocketExeption& e)
            {
                delete[] buffer2;
                throw e;
            }
            delete[] buffer2;
            data.write(buffer2,size);
        }

        return res;
    }
}

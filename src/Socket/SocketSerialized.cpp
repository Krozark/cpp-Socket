#include <Socket/SocketSerialized.hpp>
#include <Socket/define.hpp>

#include <thread>

namespace ntw {

#define HEADER_SIZE (4+2)

SocketSerialized::SocketSerialized(Socket::Dommaine dommaine,Socket::Type type,int protocole): Serializer(255), Socket(dommaine,type,protocole), status(0)
{
    //reserver les 2 premier bits pour la taille
    _cursor_end =_cursor_begin = HEADER_SIZE;
    //is_send=false;
};

SocketSerialized::SocketSerialized(Socket&& s) : Serializer(255)
{
    std::swap(s.sock,sock);
    std::swap(s.sock_cfg,sock_cfg);
    _cursor_end =_cursor_begin = HEADER_SIZE;
};

int SocketSerialized::send()
{
    //écrire la taille dans les 2 premier oct
    uint32_t size = _cursor_end - _cursor_begin;
    uint8_t *d = (uint8_t *)&size;
    #if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    _buffer[_cursor_begin - 6] = d[0];
    _buffer[_cursor_begin - 5] = d[1];
    _buffer[_cursor_begin - 4] = d[2];
    _buffer[_cursor_begin - 3] = d[3];
    #elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    _buffer[_cursor_begin - 6] = d[3];
    _buffer[_cursor_begin - 5] = d[2];
    _buffer[_cursor_begin - 4] = d[1];
    _buffer[_cursor_begin - 3] = d[0];
    #else
    #error "byte orden not suported (PDP endian)"
    #endif

    {
        uint16_t st = status;
        d = (uint8_t*)&st;
        #if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
        _buffer[_cursor_begin - 2] = d[0];
        _buffer[_cursor_begin - 1] = d[1];
        #elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        _buffer[_cursor_begin - 2] = d[1];
        _buffer[_cursor_begin - 1] = d[0];
        #else
        #error "byte orden not suported (PDP endian)"
        #endif
    }
    //envoyer
    int res = Socket::send(_buffer+_cursor_begin-HEADER_SIZE, HEADER_SIZE+size);
    //reset
    //clear();
    return res;
};

int SocketSerialized::receive()
{
    //recuperer la taille dans les 6 premier oct
    int res = Socket::receive(_buffer,HEADER_SIZE);
    if (res > 0)
    {
        uint32_t size;
        {
            uint8_t d[4];
            #if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
            d[0]= _buffer[0];
            d[1]= _buffer[1];
            d[2]= _buffer[2];
            d[3]= _buffer[3];
            #elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
            d[3]= _buffer[0];
            d[2]= _buffer[1];
            d[1]= _buffer[2];
            d[0]= _buffer[3];
            #else
            #error "byte orden not suported (PDP endian)"
            #endif
            size = *(uint32_t*)&d;
        }


        {
            uint8_t d[2];
            #if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
            d[0]= _buffer[4];
            d[1]= _buffer[5];
            #elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
            d[1]= _buffer[4];
            d[0]= _buffer[5];
            #else
            #error "byte orden not suported (PDP endian)"
            #endif
            status = *(uint16_t*)&d;
        }

        //reset
        if (int(_buffer_size) < (int)HEADER_SIZE+(int)size)
            resize(HEADER_SIZE+size);
        //else _buffer_size ne change pas
        _cursor_begin = HEADER_SIZE;
        _cursor_end = HEADER_SIZE+size;
        //remplacer le buffer
        if(size>0)
        {
            int recv_left = size;
            int recv = 0;
            while(recv_left > 0)
            {
                recv = Socket::receive(_buffer+res,recv_left);
                if(recv<=0)
                    //TODO ERROR
                    break;
                res+=recv;
                recv_left -=recv;
            }
        }
    }
    else
    {
        clear();
        setStatus(NTW_STOP_CONNEXION);
    }
    return res;
};

void SocketSerialized::setStatus(short int st)
{
    status = st;
}

short int SocketSerialized::getStatus()const
{
    return status;
}

void SocketSerialized::clear()
{
    _cursor_begin = _cursor_end = HEADER_SIZE;
}

std::ostream& operator<<(std::ostream& output,const SocketSerialized& self)
{
    output<<"[id("<<self.id()<<"),size("<<self.size()<<"),status("<<self.status<<")]";
    for(unsigned int i=self._cursor_begin; i<self._cursor_end;++i)
    {
        if(self._buffer[i] < 33 or self._buffer[i] >126)
            output<<"<"<<(int)self._buffer[i]<<">";
        else
            output<<"'"<<(char)self._buffer[i]<<"'";
    }
    return output;
};
    

};

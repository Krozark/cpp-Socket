#include <Socket/SocketSerialized.hpp>
#include <Socket/define.hpp>

namespace ntw {

SocketSerialized::SocketSerialized(Socket::Dommaine dommaine,Socket::Type type,int protocole): Serializer(255), Socket(dommaine,type,protocole), status(0)
{
    //reserver les 2 premier bits pour la taille
    _cursor_end =_cursor_begin = 4;
    //is_send=false;
};

SocketSerialized::SocketSerialized(Socket&& s) : Serializer(255)
{
    std::swap(s.sock,sock);
    std::swap(s.sock_cfg,sock_cfg);
    _cursor_end =_cursor_begin = 4;
};

void SocketSerialized::send()
{
    //écrire la taille dans les 2 premier oct
    uint16_t size = _cursor_end - _cursor_begin;
    uint8_t *d = (uint8_t *)&size;
    #if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    _buffer[_cursor_begin - 4] = d[0];
    _buffer[_cursor_begin - 3] = d[1];
    #elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
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
    //std::cout<<"send: "<<*this<<std::endl;
    int res = Socket::send(_buffer+_cursor_begin-4, 4+size);
    std::cout<<"Recv : "<<res<<"/"<<int(4+size)<<std::endl;
    //reset
    //clear();
};

int SocketSerialized::receive()
{
    //recuperer la taille dans les 4 premier oct
    int res = Socket::receive(_buffer,4);
    if (res > 0)
    {
        uint8_t d[2];
        #if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
        d[0]= _buffer[0];
        d[1]= _buffer[1];
        #elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        d[1]= _buffer[0];
        d[0]= _buffer[1];
        #else
        #error "byte orden not suported (PDP endian)"
        #endif
        uint16_t size = *(uint16_t*)&d;

        #if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
        d[0]= _buffer[2];
        d[1]= _buffer[3];
        #elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        d[1]= _buffer[2];
        d[0]= _buffer[3];
        #else
        #error "byte orden not suported (PDP endian)"
        #endif
        status = *(uint16_t*)&d;

        //reset
        if (int(_buffer_size) < 4+size)
            resize(4+size);
        //else _buffer_size ne change pas
        _cursor_begin = 4;
        _cursor_end = 4+size;
        //remplacer le buffer
        if(size>0)
            res += Socket::receive(_buffer+4,size);
        std::cout<<"Recv size: "<<size<<std::endl;
    }
    else
    {
        clear();
        setStatus(NTW_STOP_CONNEXION);
    }
    //std::cout<<"recv: "<<*this<<std::endl;
    std::cout<<"Recv real: "<<res<<std::endl;
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
    _cursor_begin = _cursor_end = 4;
}

std::ostream& operator<<(std::ostream& output,const SocketSerialized& self)
{
    output<<"["<<self.size()<<":"<<self.status<<"]";
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

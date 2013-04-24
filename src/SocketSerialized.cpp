#include "SocketSerialized.hpp"

namespace ntw {

SocketSerialized::SocketSerialized(Socket::Dommaine dommaine,Socket::Type type,int protocole): Serializer(255), Socket(dommaine,type,protocole)
{
    //reserver les 2 premier bits pour la taille
    _cursor_end =_cursor_begin = 2;
    //is_send=false;
};

SocketSerialized::SocketSerialized(Socket&& s) : Serializer(255)
{
    std::swap(s.sock,sock);
    std::swap(s.sock_cfg,sock_cfg);
    _cursor_end =_cursor_begin = 2;
};

SocketSerialized::~SocketSerialized()
{
};

void SocketSerialized::Send()
{
    //écrire la taille dans les 2 premier oct
    uint16_t size = _cursor_end - _cursor_begin;
    uint8_t *d = (uint8_t *)&size;
    #if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    _buffer[_cursor_begin - 2] = d[0];
    _buffer[_cursor_begin - 1] = d[1];
    #elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    _buffer[_cursor_begin - 2] = d[1];
    _buffer[_cursor_begin - 1] = d[0];
    #else
    #error "byte orden not suported (PDP endian)"
    #endif
    //envoyer
    Socket::Send(_buffer+_cursor_begin-2, 2+size);
    //reset
    //clear();
};

void SocketSerialized::Receive()
{
    //recuperer la taille dans les 2 premier oct
    Socket::Receive(_buffer,2);
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
    //reset
    if (_buffer_size < 2+size)
        resize(2+size);
    //else _buffer_size ne change pas
    _cursor_begin = 2;
    _cursor_end = 2+size;
    //remplacer le buffer
    Socket::Receive(_buffer+2,size);
};

void SocketSerialized::clear()
{
    _cursor_begin = _cursor_end = 2;
}

    

};

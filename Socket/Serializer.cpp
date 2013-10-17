#include <Socket/Serializer.hpp>

namespace ntw {

Serializer::Serializer(unsigned int buffer_size) : _cursor_end(0),_cursor_begin(0), _buffer_size(buffer_size)
{
   _buffer = new unsigned char[buffer_size];
};

Serializer::~Serializer()
{
    delete[] _buffer;
    /*_buffer = 0;
    _cursor_end = 0;
    _buffer_size = 0;*/
};

void Serializer::clear()
{
   _cursor_end = _cursor_begin = 0; 
};

const unsigned int Serializer::size()const
{
    return _cursor_end - _cursor_begin;
};

/********* SERIALIZE *************/
//1 oct | 8 bit
Serializer& Serializer::operator<<(char c)
{
    push(*reinterpret_cast<uint8_t*>(&c));
    return *this;
};
//2 oct | 16 bit
Serializer& Serializer::operator<<(short int s)
{
    push(*reinterpret_cast<uint16_t*>(&s));
    return *this;
};
//4 oct | 32 bit
Serializer& Serializer::operator<<(int i)
{
    push(*reinterpret_cast<uint32_t*>(&i));
    return *this;
};

Serializer& Serializer::operator<<(unsigned int i)
{
    push(*reinterpret_cast<uint32_t*>(&i));
    return *this;
};

Serializer& Serializer::operator<<(float f)
{
    push(*reinterpret_cast<uint32_t*>(&f));
    return *this;
};
//8 oct | 64 bit
Serializer& Serializer::operator<<(double d)
{
    push(*reinterpret_cast<uint64_t*>(&d));
    return *this;
};
Serializer& Serializer::operator<<(long int l)
{
    push(*reinterpret_cast<uint64_t*>(&l));
    return *this;
};
//1 oct | 8 bit []
Serializer& Serializer::operator<<(const char* const c)
{
    uint8_t* data = (uint8_t*)c;
    for(int i=0;c[i];++i) //exit when \0
        push(data[i]);
    uint8_t end = '\0';
    push(end);
    return *this;
};

Serializer& Serializer::operator<<(const std::string& str)
{
    return (*this<<str.c_str());
}

/********** UNSERIALIZE ***********/
//1 oct | 8 bit
Serializer& Serializer::operator>>(char& c)
{
    pop(*reinterpret_cast<uint8_t*>(&c));
    return *this;
}
//2 oct | 16 bit
Serializer& Serializer::operator>>(short int s)
{
    pop(*reinterpret_cast<uint16_t*>(&s));
    return *this;
};
//4 oct | 32 bit
Serializer& Serializer::operator>>(int& i)
{
    pop(*reinterpret_cast<uint32_t*>(&i));
    return *this;
};

Serializer& Serializer::operator>>(unsigned int& i)
{
    pop(*reinterpret_cast<uint32_t*>(&i));
    return *this;
};

Serializer& Serializer::operator>>(float& f)
{
    pop(*reinterpret_cast<uint32_t*>(&f));
    return *this;
};
//8 oct | 64 bit
Serializer& Serializer::operator>>(double& d)
{
    pop(*reinterpret_cast<uint64_t*>(&d));
    return *this;
};
Serializer& Serializer::operator>>(long int& l)
{
    pop(*reinterpret_cast<uint64_t*>(&l));
    return *this;
};
//1 oct | 8 bit []
Serializer& Serializer::operator>>(char*& c)
{
    unsigned int size = _cursor_begin;
    for(;size<_cursor_end && _buffer[size] != '\0';++size){}
    size -= _cursor_begin-1;

    char* data = new char[size];

    for(unsigned int i=0;i<size;++i)
        data[i] = _buffer[_cursor_begin + i];

    _cursor_begin += size;

    if (c)
        delete[] c;
    c=data;

    return *this;
};

Serializer& Serializer::operator>>(std::string& str)
{
    unsigned int size = _cursor_begin;
    for(;size<_cursor_end && _buffer[size] != '\0';++size){}
    size -= _cursor_begin-1;

    str = std::string((char *)(_buffer+_cursor_begin),size);

    _cursor_begin += size;

    return *this;
};



std::ostream& operator<<(std::ostream& output,const Serializer& self)
{
    output<<"["<<self.size()<<"]";
    for(int i=self._cursor_begin; i<self._cursor_end;++i)
        output<<"<"<<(int)self._buffer[i]<<">";
    return output;
};

};

#include <Socket/Serializer.hpp>
#include <cstdio>
#include <sys/stat.h>

namespace ntw {

Serializer::Serializer(const unsigned int buffer_size) : _cursor_end(0),_cursor_begin(0), _buffer_size(buffer_size)
{
   _buffer = new unsigned char[buffer_size];
};

Serializer::~Serializer()
{
    delete[] _buffer;
    /*_buffer = 0;
    _cursor_end = 0;
    _buffer_size = 0;*/
}

void Serializer::clear(const unsigned int buffer_size)
{
    _cursor_end = _cursor_begin = 0; 
    _buffer_size = buffer_size;
    delete[] _buffer;
    _buffer = new unsigned char[buffer_size];
}

unsigned int Serializer::size()const
{
    return _cursor_end - _cursor_begin;
}

unsigned int Serializer::capacity()const
{
    return _buffer_size;
}

const unsigned char* Serializer::data()const
{
    return _buffer + _cursor_begin;
}

unsigned char* Serializer::data()
{
    return _buffer + _cursor_begin;
}


Serializer& Serializer::write(const void* buffer,unsigned int size)
{
    while(_buffer_size < _cursor_end + size)
        resize(_buffer_size*2);
    ::memcpy(_buffer+_cursor_end,buffer,size);
    _cursor_end +=size;
    return *this;
}

Serializer& Serializer::read(void* buffer,unsigned int size)
{
    if(_cursor_begin + size <= _cursor_end)
    {
        ::memcpy(buffer,_buffer+_cursor_begin,size);
        _cursor_begin += size;
    }
    return *this;
}

bool Serializer::save(const std::string& filename,bool append)const
{
    FILE* file = nullptr;
    if(append)
        file = ::fopen(filename.c_str(),"ab");
    else
        file = ::fopen(filename.c_str(),"wb");

    if(file == NULL)
        return false;
    unsigned int begin = _cursor_begin;

    while(begin + BUFSIZ < _cursor_end)
    {
        ::fwrite(_buffer+begin,BUFSIZ,1,file);
        begin+=BUFSIZ;
    }

    int size = _cursor_end - begin;
    if(size>0)
    {
        ::fwrite(_buffer+begin,size,1,file);
    }

    ::fclose(file);
    return true;
}

bool Serializer::load(const std::string& filename)
{
    FILE* file = ::fopen(filename.c_str(),"rb");

    if(file == NULL)
        return false;

    struct stat st;
    if(stat(filename.c_str(), &st) != 0)
        return false;

    int size = st.st_size;

    while(_buffer_size < _cursor_end + size)
        this->resize(_buffer_size*2);

    if(::fread(_buffer+_cursor_end,size,1,file) !=0)
    {
        _cursor_end+=size;
    }
    else
        return 0;

    ::fclose(file);
    return true;
}

/********* SERIALIZE *************/
//1 oct | 8 bit []
Serializer& Serializer::operator<<(const std::string& str)
{
    const char* c = str.c_str();
    const uint8_t* data = (const uint8_t*)c;
    for(int i=0;c[i];++i) //exit when \0
        push(data[i]);
    uint8_t end = '\0';
    push(end);
    return *this;
};

/********** UNSERIALIZE ***********/
Serializer& Serializer::operator<<(char* str)
{
    return (*this<<std::string(str));
}


Serializer& Serializer::operator>>(std::string& str)
{
    unsigned int size = _cursor_begin;
    for(;size<_cursor_end && _buffer[size] != '\0';++size){}
    size -= _cursor_begin;//-1 for \0

    str = std::string((char *)(_buffer+_cursor_begin),size);

    _cursor_begin += size +1;

    return *this;
};

std::ostream& operator<<(std::ostream& output,const Serializer& self)
{
    output<<"["<<self.size()<<"]";
    for(unsigned int i=self._cursor_begin; i<self._cursor_end;++i)
    {
        if(self._buffer[i] < 33 or self._buffer[i] >126)//ascii code for readable char
            output<<"<"<<(int)self._buffer[i]<<">";
        else
            output<<"'"<<(char)self._buffer[i]<<"'";
    }
    return output;
};


};

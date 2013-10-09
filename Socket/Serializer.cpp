#include <Socket/Serializer.hpp>

namespace ntw {

Serializer::Serializer(unsigned int buffer_size) : _cursor_end(0),_cursor_begin(0), _buffer_size(buffer_size)
{
   _buffer = new unsigned char[buffer_size];
};

Serializer::~Serializer()
{
    delete _buffer;
    /*_buffer = 0;
    _cursor_end = 0;
    _buffer_size = 0;*/
};

void Serializer::clear()
{
   _cursor_end = _cursor_begin = 0; 
};



std::ostream& operator<<(std::ostream& output,const Serializer& self)
{
    output<<"["<<self.size()<<"]";
    for(int i=self._cursor_begin; i<self._cursor_end;++i)
        output<<"<"<<(int)self._buffer[i]<<">";
    return output;
};

};

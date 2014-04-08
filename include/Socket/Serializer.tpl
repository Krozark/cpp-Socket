#include <utility>
namespace ntw
{
    ///////////////// append ///////////////////////////
    template<typename T,size_t N>
    Serializer& Serializer::operator<<(const std::array<T,N>& container)
    {
        const unsigned int size = N;
        *this<<size;
        for(unsigned int i=0;i<size;++i)
            *this<<container[i];
        return *this;
    }

    template<typename T>
    Serializer& Serializer::operator<<(const std::vector<T>& container)
    {
        const unsigned int size = container.size();
        *this<<size;
        for(unsigned int i=0;i<size;++i)
            *this<<container[i];
        return *this;
    }

    template<typename T>
    Serializer& Serializer::operator<<(const std::list<T>& container)
    {
        const unsigned int size = container.size();
        *this<<size;
        for(const auto& c : container)
            *this<<c;
        return *this;
    }


    //remove
    template<typename T>
    Serializer& Serializer::operator>>(std::vector<T>& container)
    {
        unsigned int size = 0;
        *this>>size;
        for(unsigned int i=0;i<size;++i)
        {
            T tmp;
            *this>>tmp;
            container.push_back(std::move(tmp));
        }
        return *this;
    }

    template<typename T>
    Serializer& Serializer::operator>>(std::list<T>& container)
    {
        unsigned int size = 0;
        *this>>size;
        for(unsigned int i=0;i<size;++i)
        {
            T tmp;
            *this>>tmp;
            container.push_back(std::move(tmp));
        }
        return *this;
    }


    //////////// utility //////////////////
    void Serializer::resize(const unsigned int buffer_cursor_end)
    {
        unsigned char* tmp = new unsigned char[buffer_cursor_end];
        memcpy(tmp,_buffer,_buffer_size);

        delete[] _buffer;
        _buffer = tmp;
        _buffer_size = buffer_cursor_end;
    }

    void Serializer::push(const uint8_t& a)
    {
        if(_buffer_size < _cursor_end + 1)
            resize(_buffer_size*2);

        _buffer[_cursor_end++] = a;
    }

    void Serializer::push(const uint16_t& a)
    {
        if(_buffer_size < _cursor_end + 2)
            resize(_buffer_size*2);

        const uint8_t *d = (const uint8_t *)&a;

        #if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
        _buffer[_cursor_end++] = d[0];
        _buffer[_cursor_end++] = d[1];
        #elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        _buffer[_cursor_end++] = d[1];
        _buffer[_cursor_end++] = d[0];
        #else
        #error "byte orden not suported (PDP endian)"
        #endif

    }

    void Serializer::push(const uint32_t& a)
    {
        if(_buffer_size < _cursor_end + 4)
            resize(_buffer_size*2);

        const uint8_t *d = (const uint8_t *)&a;

        #if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
        _buffer[_cursor_end++] = d[0];
        _buffer[_cursor_end++] = d[1];
        _buffer[_cursor_end++] = d[2];
        _buffer[_cursor_end++] = d[3];
        #elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        _buffer[_cursor_end++] = d[3];
        _buffer[_cursor_end++] = d[2];
        _buffer[_cursor_end++] = d[1];
        _buffer[_cursor_end++] = d[0];
        #else
        #error "byte orden not suported (PDP endian)"
        #endif

    }

    void Serializer::push(const uint64_t& a)
    {
        if(_buffer_size < _cursor_end + 8)
            resize(_buffer_size*2);

        const uint8_t *d = (const uint8_t *)&a;

        #if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
        _buffer[_cursor_end++] = d[0];
        _buffer[_cursor_end++] = d[1];
        _buffer[_cursor_end++] = d[2];
        _buffer[_cursor_end++] = d[3];
        _buffer[_cursor_end++] = d[4];
        _buffer[_cursor_end++] = d[5];
        _buffer[_cursor_end++] = d[6];
        _buffer[_cursor_end++] = d[7];
        #elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        _buffer[_cursor_end++] = d[7];
        _buffer[_cursor_end++] = d[6];
        _buffer[_cursor_end++] = d[5];
        _buffer[_cursor_end++] = d[4];
        _buffer[_cursor_end++] = d[3];
        _buffer[_cursor_end++] = d[2];
        _buffer[_cursor_end++] = d[1];
        _buffer[_cursor_end++] = d[0];
        #else
        #error "byte orden not suported (PDP endian)"
        #endif
    }

    void Serializer::pop(uint8_t& a)
    {
        if(_cursor_begin +1 <= _cursor_end )
        {
            a= _buffer[_cursor_begin++];
        }
    }

    void Serializer::pop(uint16_t& a)
    {
        if(_cursor_begin +2 <= _cursor_end)
        {
            uint8_t *d = (uint8_t *)&a;
            #if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
            d[0]= _buffer[_cursor_begin++];
            d[1]= _buffer[_cursor_begin++];
            #elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
            d[1]= _buffer[_cursor_begin++];
            d[0]= _buffer[_cursor_begin++];
            #else
            #error "byte orden not suported (PDP endian)"
            #endif
        }
    }

    void Serializer::pop(uint32_t& a)
    {
        if(_cursor_begin +4 <= _cursor_end)
        {
            uint8_t *d = (uint8_t *)&a;
            #if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
            d[0]= _buffer[_cursor_begin++];
            d[1]= _buffer[_cursor_begin++];
            d[2]= _buffer[_cursor_begin++];
            d[3]= _buffer[_cursor_begin++];
            #elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
            d[3]= _buffer[_cursor_begin++];
            d[2]= _buffer[_cursor_begin++];
            d[1]= _buffer[_cursor_begin++];
            d[0]= _buffer[_cursor_begin++];
            #else
            #error "byte orden not suported (PDP endian)"
            #endif
        }
    }

    void Serializer::pop(uint64_t& a)
    {
        if(_cursor_begin +8 <= _cursor_end)
        {
            uint8_t *d = (uint8_t *)&a;
            #if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
            d[0]= _buffer[_cursor_begin++];
            d[1]= _buffer[_cursor_begin++];
            d[2]= _buffer[_cursor_begin++];
            d[3]= _buffer[_cursor_begin++];
            d[4]= _buffer[_cursor_begin++];
            d[5]= _buffer[_cursor_begin++];
            d[6]= _buffer[_cursor_begin++];
            d[7]= _buffer[_cursor_begin++];
            #elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
            d[7]= _buffer[_cursor_begin++];
            d[6]= _buffer[_cursor_begin++];
            d[5]= _buffer[_cursor_begin++];
            d[4]= _buffer[_cursor_begin++];
            d[3]= _buffer[_cursor_begin++];
            d[2]= _buffer[_cursor_begin++];
            d[1]= _buffer[_cursor_begin++];
            d[0]= _buffer[_cursor_begin++];
            #else
            #error "byte orden not suported (PDP endian)"
            #endif
        }
    }
}

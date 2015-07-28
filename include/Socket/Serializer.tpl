#include <utility>
namespace ntw
{
    ///////////////// append ///////////////////////////
    template<typename T>
    Serializer& Serializer::operator<<(const T& v)
    {
        push(v);
        return *this;
    }

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
    Serializer& Serializer::operator>>(T& v)
    {
        pop(v);
        return *this;
    }

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

    template <typename T> struct Serializer::Size<const T>{
        enum {value = Serializer::Size<T>::value};
    };

    template <> struct Serializer::Size<bool> {
        enum {value = 1};
    };
    template <> struct Serializer::Size<char> {
        enum {value = 1};
    };
    template <> struct Serializer::Size<uint8_t> {
        enum {value = 1};
    };

    template <> struct Serializer::Size<short int>{
        enum {value = 2};
    };
    template <> struct Serializer::Size<uint16_t> {
        enum {value = 2};
    };

    template <> struct Serializer::Size<int>{
        enum {value = 4};
    };
    template <> struct Serializer::Size<unsigned int>{
        enum {value = 4};
    };
    template <> struct Serializer::Size<float>{
        enum {value = 4};
    };
    /*template <> struct Serializer::Size<uint32_t>{
        enum {value = 4};
    };*/

    template <> struct Serializer::Size<double>{
        enum {value = 8};
    };
    template <> struct Serializer::Size<long int>{
        enum {value = 8};
    };
    template <> struct Serializer::Size<uint64_t>{
        enum {value = 8};
    };

    template <typename T>
    void Serializer::convert(const T& value,char* buffer)
    {
        const char *d = reinterpret_cast<const char*>(&value);

        #if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
        for(int i = 0; i< Serializer::Size<T>::value; ++i)
            buffer[i] = d[i];
        #elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        int cursor = 0;
        for(int i = Serializer::Size<T>::value -1; i>=0; --i,++cursor)
            buffer[cursor] = d[i];
        #else
        #error "byte orden not suported (PDP endian)"
        #endif
    }

    template <typename T>
    void Serializer::convert(const char* buffer,T& res)
    {
        char *d = reinterpret_cast<char*>(&res);

        #if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
        for(int i = 0; i<Serializer::Size<T>::value; ++i)
            d[i]= buffer[i];
        #elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        int cursor = 0;
        for(int i =Serializer::Size<T>::value-1;i>=0;--i,++cursor)
            d[i]= buffer[cursor];
        #else
        #error "byte orden not suported (PDP endian)"
        #endif
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

    template <typename T>
    void Serializer::push(const T& a)
    {
        while(_buffer_size < _cursor_end + Serializer::Size<T>::value)
            resize(_buffer_size*2);

        const char *d = (const char*)(&a);

        #if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
        for(int i = 0; i< Serializer::Size<T>::value; ++i)
            _buffer[_cursor_end++] = d[i];
        #elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        for(int i = Serializer::Size<T>::value -1; i>=0; --i)
            _buffer[_cursor_end++] = d[i];
        #else
        #error "byte orden not suported (PDP endian)"
        #endif

    }

    template <typename T>
    void Serializer::pop(T& a)
    {
        if(_cursor_begin +Serializer::Size<T>::value <= _cursor_end)
        {
            char *d = (char*)(&a);
            #if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
            for(int i = 0; i<Serializer::Size<T>::value; ++i)
                d[i]= _buffer[_cursor_begin++];
            #elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
            for(int i =Serializer::Size<T>::value-1;i>=0;--i)
                d[i]= _buffer[_cursor_begin++];
            #else
            #error "byte orden not suported (PDP endian)"
            #endif
        }
    }
}

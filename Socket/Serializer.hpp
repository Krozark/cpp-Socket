#ifndef NTW_SERIALIZER_HPP
#define NTW_SERIALIZER_HPP

#include <string.h>
#include <stdint.h>
#include <iostream>


#include <stdio.h>

/*
 * Se base sur .. pour les fonction en interne
__BYTE_ORDER__
__ORDER_LITTLE_ENDIAN__
__ORDER_BIG_ENDIAN__
__ORDER_PDP_ENDIAN__
*/

#if __BYTE_ORDER__ == __ORDER_PDP_ENDIAN__
#error "byte orden not suported (PDP endian)"
#endif

namespace ntw {
/* Convert local data in BIG ENDIAN for network, and from big endian to local  */
class Serializer
{
    public:
        explicit Serializer(unsigned int buffer_size=255);
        ~Serializer();

        void clear();
        
        Serializer(const Serializer&) = delete;
        Serializer& operator=(const Serializer&) = delete;


        inline const unsigned int size()const{return _cursor_end - _cursor_begin;};

        /********* SERIALIZE *************/
        //1 oct | 8 bit
        inline Serializer& operator<<(char c){push(*reinterpret_cast<uint8_t*>(&c));return *this;};         
        //2 oct | 16 bit
        inline Serializer& operator<<(short int s){push(*reinterpret_cast<uint16_t*>(&s));return *this;};
        //4 oct | 32 bit
        inline Serializer& operator<<(int i){push(*reinterpret_cast<uint32_t*>(&i));return *this;};
        inline Serializer& operator<<(float f){push(*reinterpret_cast<uint32_t*>(&f));return *this;};
        //8 oct | 64 bit
        inline Serializer& operator<<(double d){push(*reinterpret_cast<uint64_t*>(&d));return *this;};
        inline Serializer& operator<<(long int l){push(*reinterpret_cast<uint64_t*>(&l));return *this;};
        //16 oct | 124 bit
        //inline Serializer& operator<<(long double ld){push(*reinterpret_cast<uint128_t*>(&ld);};
        //1 oct | 8 bit []
        inline Serializer& operator<<(const char* const c){
            uint8_t* data = (uint8_t*)c;
            for(int i=0;c[i];++i) //exit when \0
                push(data[i]);
            uint8_t end = '\0';
            push(end);
            return *this;
        };

        /********** UNSERIALIZE ***********/
        //1 oct | 8 bit
        inline Serializer& operator>>(char& c){pop(*reinterpret_cast<uint8_t*>(&c));return *this;};         
        //2 oct | 16 bit
        inline Serializer& operator>>(short int s){pop(*reinterpret_cast<uint16_t*>(&s));return *this;};
        //4 oct | 32 bit
        inline Serializer& operator>>(int& i){pop(*reinterpret_cast<uint32_t*>(&i));return *this;};
        inline Serializer& operator>>(float& f){pop(*reinterpret_cast<uint32_t*>(&f));return *this;};
        //8 oct | 64 bit
        inline Serializer& operator>>(double& d){pop(*reinterpret_cast<uint64_t*>(&d));return *this;};
        inline Serializer& operator>>(long int& l){pop(*reinterpret_cast<uint64_t*>(&l));return *this;};
        //1 oct | 8 bit []
        inline Serializer& operator>>(char*& c){
            unsigned int size = _cursor_begin;
            for(;size<_cursor_end && _buffer[size] != '\0';++size){}
            size -= _cursor_begin-1;

            char* data = new char[size];

            for(unsigned int i=0;i<size;++i)
                data[i] = _buffer[_cursor_begin + i];

            _cursor_begin += size;

            if (c)
                delete c;
            c=data;

            return *this;
        };


        friend std::ostream& operator<<(std::ostream& output,const Serializer& self);


    protected:

        unsigned char* _buffer;
        unsigned int _cursor_end;
        unsigned int _cursor_begin;
        unsigned int _buffer_size;

        inline void resize(const unsigned int buffer_cursor_end)
        {
            unsigned char* buffer = new unsigned char[buffer_cursor_end];
            buffer = (unsigned char*)memcpy(buffer,_buffer,_buffer_size);

            delete _buffer;
            _buffer = buffer;
            _buffer_size = buffer_cursor_end;
        };


        /************ ADD DATAs ***************/

        inline void push(uint8_t& a){
            if(_buffer_size < _cursor_end + 1)
                resize(_buffer_size+128);

            _buffer[_cursor_end++] = a;
        };

        inline void push(uint16_t& a) {
            if(_buffer_size < _cursor_end + 2)
                resize(_buffer_size+128);

            uint8_t *d = (uint8_t *)&a;

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

        inline void push(uint32_t& a){
            if(_buffer_size < _cursor_end + 4)
                resize(_buffer_size+128);

            uint8_t *d = (uint8_t *)&a;

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

        inline void push(uint64_t& a){
            if(_buffer_size < _cursor_end + 8)
                resize(_buffer_size+128);

            uint8_t *d = (uint8_t *)&a;

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

        /***************** GET DATAs ****************/
        inline void pop(uint8_t& a){
            if(_cursor_begin +1 <= _cursor_end )
            {
                a= _buffer[_cursor_begin++];
            }
        };

        inline void pop(uint16_t& a){
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
        };

        inline void pop(uint32_t& a){
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
        };

        inline void pop(uint64_t& a){
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
        };
};

};
#endif


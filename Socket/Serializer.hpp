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


        const unsigned int size()const;

        /********* SERIALIZE *************/
        //1 oct | 8 bit
        Serializer& operator<<(char c);
        //2 oct | 16 bit
        Serializer& operator<<(short int s);
        //4 oct | 32 bit
        Serializer& operator<<(int i);
        Serializer& operator<<(unsigned int i);
        Serializer& operator<<(float f);
        //8 oct | 64 bit
        Serializer& operator<<(double d);
        Serializer& operator<<(long int l);
        //16 oct | 124 bit
        //Serializer& operator<<(long double ld){push(*reinterpret_cast<uint128_t*>(&ld);};
        //1 oct | 8 bit []
        Serializer& operator<<(const char* const c);
        //string
        Serializer& operator<<(const std::string& str);

        /********** UNSERIALIZE ***********/
        //1 oct | 8 bit
        Serializer& operator>>(char& c);
        //2 oct | 16 bit
        Serializer& operator>>(short int s);
        //4 oct | 32 bit
        Serializer& operator>>(int& i);
        Serializer& operator>>(unsigned int& i);
        Serializer& operator>>(float& f);
        //8 oct | 64 bit
        Serializer& operator>>(double& d);
        Serializer& operator>>(long int& l);
        //1 oct | 8 bit []
        Serializer& operator>>(char*& c);
        Serializer& operator>>(std::string& str);


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


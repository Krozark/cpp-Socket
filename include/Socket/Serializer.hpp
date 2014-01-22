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
/**
 * A class that Convert local data in BIG ENDIAN for network, and from big endian to local ordering
 */
class Serializer
{
    public:
        /**
         * \brief Constructor
         * \param buffer_size the internal initial buffer size
         */
        explicit Serializer(unsigned int buffer_size=255);

        /**
         * \brief Destructor
         */
        ~Serializer();

        /**
         * \brief clear the buffe, but nor free it
         */
        void clear();
        
        Serializer(const Serializer&) = delete;
        Serializer& operator=(const Serializer&) = delete;


        /**
         * \return The number of byts stored
         */
        const unsigned int size()const;

        /********* SERIALIZE *************/
        Serializer& operator<<(const char c); ///< Oveload operator to stor data. 1 oct | 8 bit

        Serializer& operator<<(const short int s);///< Oveload operator to stor data. 2 oct | 16 bit

        Serializer& operator<<(const int i);///< Oveload operator to stor data. 4 oct | 32 bit
        Serializer& operator<<(const unsigned int i);///< Oveload operator to stor data.  4 oct | 32 bit
        Serializer& operator<<(const float f);///< Oveload operator to stor data.  4 oct | 32 bit

        Serializer& operator<<(const double d);///< Oveload operator to stor data. 8 oct | 64 bit
        Serializer& operator<<(const long int l);///< Oveload operator to stor data. 8 oct | 64 bit
        
        //16 oct | 124 bit
        //Serializer& operator<<(long double ld){push(*reinterpret_cast<uint128_t*>(&ld);};
        
        Serializer& operator<<(const char* const c);///< Oveload operator to stor data. \0 is the end of data. 1 oct | 8 bit []
        Serializer& operator<<(const std::string& str);///< Oveload operator to stor data. \0 is the end of data. 1 oct | 8 bit []


        /********** UNSERIALIZE ***********/
        Serializer& operator>>(char& c); ///< Oveload operator to extract datas. 1 oct | 8 bit

        Serializer& operator>>(short int s); ///< Oveload operator to extract datas. 2 oct | 16 bit

        Serializer& operator>>(int& i); ///< Oveload operator to extract datas. 4 oct | 32 bit
        Serializer& operator>>(unsigned int& i); ///< Oveload operator to extract datas. 4 oct | 32 bit
        Serializer& operator>>(float& f); ///< Oveload operator to extract datas.4 oct | 32 bit

        Serializer& operator>>(double& d); ///< Oveload operator to extract datas. 8 oct | 64 bit
        Serializer& operator>>(long int& l); ///< Oveload operator to extract datas. 8 oct | 64 bit

        Serializer& operator>>(char*& c); ///< Oveload operator to extract datas. 1 oct | 8 bit []
        Serializer& operator>>(std::string& str); ///< Oveload operator to extract datas. 1 oct | 8 bit []


        friend std::ostream& operator<<(std::ostream& output,const Serializer& self); ///<print each byt by his int value between <>


    protected:

        unsigned char* _buffer; ///< internal buffer
        unsigned int _cursor_end; ///< end cursor
        unsigned int _cursor_begin; ///< start cursor
        unsigned int _buffer_size;///< size of the buffer

        /**
         * \brief realoc the buffer
         * \param buffer_cursor_end the size to alloc
         */
        inline void resize(const unsigned int buffer_cursor_end)
        {
            unsigned char* buffer = new unsigned char[buffer_cursor_end];
            buffer = (unsigned char*)memcpy(buffer,_buffer,_buffer_size);

            delete[] _buffer;
            _buffer = buffer;
            _buffer_size = buffer_cursor_end;
        };


        /************ ADD DATAs ***************/
        /**
         * \brief Helper function to add datas
         * and convert it in big endian
         * \param a data to add
         */
        inline void push(const uint8_t& a){
            if(_buffer_size < _cursor_end + 1)
                resize(_buffer_size+128);

            _buffer[_cursor_end++] = a;
        };

        /**
         * \brief Helper function to add datas
         * and convert it in big endian
         * \param a data to add
         */
        inline void push(const uint16_t& a) {
            if(_buffer_size < _cursor_end + 2)
                resize(_buffer_size+128);

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

        /**
         * \brief Helper function to add datas
         * and convert it in big endian
         * \param a data to add
         */
        inline void push(const uint32_t& a){
            if(_buffer_size < _cursor_end + 4)
                resize(_buffer_size+128);

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

        /**
         * \brief Helper function to add datas
         * and convert it in big endian
         * \param a data to add
         */
        inline void push(const uint64_t& a){
            if(_buffer_size < _cursor_end + 8)
                resize(_buffer_size+128);

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

        /***************** GET DATAs ****************/
        /**
         * \brief Helper function to get datas
         * and convert it from big endian to local encoding
         * \param a data to get
         */
        inline void pop(uint8_t& a){
            if(_cursor_begin +1 <= _cursor_end )
            {
                a= _buffer[_cursor_begin++];
            }
        };

        /**
         * \brief Helper function to get datas
         * and convert it from big endian to local encoding
         * \param a data to get
         */
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

        /**
         * \brief Helper function to get datas
         * and convert it from big endian to local encoding
         * \param a data to get
         */
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

        /**
         * \brief Helper function to get datas
         * and convert it from big endian to local encoding
         * \param a data to get
         */
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


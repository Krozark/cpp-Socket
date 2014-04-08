#ifndef NTW_SERIALIZER_HPP
#define NTW_SERIALIZER_HPP

#include <string.h>
#include <stdint.h>
#include <iostream>

#include <stdio.h>

#include <array>
#include <vector>
#include <list>
#include <fstream>

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
         * \param buffer_size the internal initial buffer size (must be >0)
         */
        explicit Serializer(const unsigned int buffer_size=256);

        /**
         * \brief Destructor
         */
        virtual ~Serializer();

        /**
         * \brief clear the buffer, and resize it, but nor free it
         * \param buffer_size the initial buffer size
         */
        void clear(const unsigned int buffer_size=256);
        
        Serializer(const Serializer&) = delete;
        Serializer& operator=(const Serializer&) = delete;


        /**
         * \return The number of byts stored
         */
        const unsigned int size()const;

        /**
         * \return the current buffer capacity
         */
        const unsigned int capacity()const;

        /**
        * \brief write datas as-it
        * Note : only the data are append to the internal buffer
        * \param buffer the buffer to write
        * \param the number of bits to write
        * \return *this
        */
        Serializer& write(const void* buffer,unsigned int size);

        /**
        * \brief get datas as-it
        * Note : you need to know the exact size you want to exctract.
        * \param buffer the buffer to write in
        * \param the number of bits to get
        * \return *this
        */
        Serializer& read(void* buffer,unsigned int size);

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
        
        //containers
        template<typename T,size_t N> Serializer& operator<<(const std::array<T,N>& containers);///< Overload operator to store a array of T
        template<typename T> Serializer& operator<<(const std::vector<T>& container);///< Overload operator to store a vector of T
        template<typename T> Serializer& operator<<(const std::list<T>& container);///< Overload operator to store a list of T
        //file
        //Serializer& operator<<(const std::fstream& datas);


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

        //containers
        template<typename T> Serializer& operator>>(std::vector<T>& container);///< Overload operator to store a vector of T
        template<typename T> Serializer& operator>>(std::list<T>& container);///< Overload operator to store a list of T


        //debug
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
        inline void resize(const unsigned int buffer_cursor_end);


        /************ ADD DATAs ***************/
        /**
         * \brief Helper function to add datas
         * and convert it in big endian
         * \param a data to add
         */
        inline void push(const uint8_t& a);

        /**
         * \brief Helper function to add datas
         * and convert it in big endian
         * \param a data to add
         */
        inline void push(const uint16_t& a);

        /**
         * \brief Helper function to add datas
         * and convert it in big endian
         * \param a data to add
         */
        inline void push(const uint32_t& a);

        /**
         * \brief Helper function to add datas
         * and convert it in big endian
         * \param a data to add
         */
        inline void push(const uint64_t& a);

        /***************** GET DATAs ****************/
        /**
         * \brief Helper function to get datas
         * and convert it from big endian to local encoding
         * \param a data to get
         */
        inline void pop(uint8_t& a);

        /**
         * \brief Helper function to get datas
         * and convert it from big endian to local encoding
         * \param a data to get
         */
        inline void pop(uint16_t& a);

        /**
         * \brief Helper function to get datas
         * and convert it from big endian to local encoding
         * \param a data to get
         */
        inline void pop(uint32_t& a);

        /**
         * \brief Helper function to get datas
         * and convert it from big endian to local encoding
         * \param a data to get
         */
        inline void pop(uint64_t& a);
};
};

#include <Socket/Serializer.tpl>
#endif


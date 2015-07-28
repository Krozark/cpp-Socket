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
        unsigned int size()const;

        /**
         * \return the current buffer capacity
         */
        unsigned int capacity()const;

        const unsigned char* data()const;
        
        unsigned char* data();

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


        /**
        * \brief save the serializer as a file
        * Note : do not clean the buffer.
        * \param filname the folder destination must exist
        * \bool append if true, the content is append to the local file
        * \return true if no error
        */
        bool save(const std::string& filename,bool append=false)const;

        /**
        * \brief Load a serializer from file
        * Note : this append the file data to the internal buffer.
        * \param filname the file to use as source
        * \return true if no error
        */
        bool load(const std::string& filename);

        /********* SERIALIZE *************/
        template<typename T>
        Serializer& operator<<(const T& v);
        
        Serializer& operator<<(char* str);
        Serializer& operator<<(const std::string& str);
        //containers
        template<typename T,size_t N> Serializer& operator<<(const std::array<T,N>& containers);///< Overload operator to store a array of T
        template<typename T> Serializer& operator<<(const std::vector<T>& container);///< Overload operator to store a vector of T
        template<typename T> Serializer& operator<<(const std::list<T>& container);///< Overload operator to store a list of T
        //file
        //Serializer& operator<<(const std::fstream& datas);


        /********** UNSERIALIZE ***********/
        template<typename T>
        Serializer& operator>>(T& v);

        Serializer& operator>>(std::string& str);

        //containers
        template<typename T> Serializer& operator>>(std::vector<T>& container);///< Overload operator to store a vector of T
        template<typename T> Serializer& operator>>(std::list<T>& container);///< Overload operator to store a list of T

        //debug
        friend std::ostream& operator<<(std::ostream& output,const Serializer& self); ///<print each byt by his int value between <>

        /**
         * \brief a template stuct with a ::value attr.
         * This struct hold the internal size of a type in the buffer.
         * You can use it if you need to use read() and write() function to know the size.
         * You can use it with primitive type only
         * (char,short int,int, unsigned int,float,double, long int)
         */
        template <typename T> struct Size;

        /**
         * \brief Convert T into BIG ENDIAN, and store the result into the buffer.
         * the buffer need to have a minimal size of Serializer::Size<T>::value.
         */
        template <typename T>
        static void convert(const T& value,char* buffer);

        /**
         * \brief Convert value in buffer from BIG ENDIAN, and store the result into the result.
         * the buffer need to have a minimal size of Serializer::Size<T>::value.
         */
        template <typename T>
        static void convert(const char* buffer,T& res);


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
        template <typename T>
        inline void push(const T& a);

        /***************** GET DATAs ****************/
        /**
         * \brief Helper function to get datas
         * and convert it from big endian to local encoding
         * \param a data to get
         */
        template <typename T>
        inline void pop(T& a);
};
};

#include <Socket/Serializer.tpl>
#endif


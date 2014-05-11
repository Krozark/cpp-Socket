#ifndef NTW_STATUS_HPP
#define NTW_STATUS_HPP

namespace ntw
{
    /**
     * \brief A class to handle status response
     */
    class Status
    {
        public:
            Status(short int c);///< constructor

            /*friend Serializer& operator<<(Serializer& stream,const Status& self)
              {
              stream<<self.code;
              return stream;
              }

              friend Serializer& operator>>(Serializer& stream,const Status& self)
              {
              stream>>self.code;
              return stream;
              }*/
            short int code; ///< code

            /**
             * \brief code values
             * You can use yours as long as there a greater than user_define
             */
            enum : short int {
                stop = -3,
                wrong_id = -2,
                ok = 0,
                request_add = 1,
                broadcast_add = 2,
                timeout = 3,
                select = 4,
                connexion = 5,
                unknow = 404,
                user_define
            };
    };
}

#endif

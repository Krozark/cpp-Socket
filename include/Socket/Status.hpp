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

            //friend Serializer& operator<<(Serializer& stream,const Status& self);

            short int code; ///< code

            /**
             * \brief code values
             * You can use yours as long as there a greater than user_define
             */
            enum : short int {
                stop = -3, ///< set if the connexion is lost
                wrong_id = -2, ///< set if a wrong function id is used with Client::call
                ok = 0, //< all is good
                request_add = 1,
                broadcast_add = 2,
                timeout = 3,
                select = 4,
                connexion = 5,///< set with sock.connect()
                unknow = 404,
                user_define ///< the maximal value use by the module. You can define yours after this one
            };
    };
}

#endif

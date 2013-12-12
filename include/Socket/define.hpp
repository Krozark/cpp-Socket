#ifndef NTW_DEFINE_HPP
#define NTW_DEFINE_HPP

#define NTW_MAX_CONNEXION 5
#define NTW_PORT_SERVER 3987
#define NTW_PORT_CLIENT 3988

#define NTW_VERSION 42

#define NTW_DEFAULT_TIMEOUT 5.f


/**************************
 * ERRORS MESSAGE / code
 * ***********************/

#define NTW_ERROR_UNKNOW_MSG "Unknow error"
#define NTW_ERROR_UNKNOW 404

#define NTW_WELCOM_MSG "hello!"
#define NTW_ERROR_NO 0

#define NTW_ERROR_REQUEST_ADD_MSG "ERROR: to much client connected"
#define NTW_ERROR_REQUEST_ADD 1

#define NTW_ERROR_BROADCAST_ADD_MSG "ERROR: connection to broadcast not etablish"
#define NTW_ERROR_BROADCAST_ADD 2

#define NTW_ERROR_TIMEOUT 3
#define NTW_ERROR_SELECT 4
#define NTW_ERROR_CONNEXION 5

#endif

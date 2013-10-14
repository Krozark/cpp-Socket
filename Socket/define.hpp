#ifndef NTW_DEFINE_HPP
#define NTW_DEFINE_HPP

#define NTW_CLIENT  1
#define NTW_SERVER  2

#ifndef NTW_MODE
#define NTW_MODE NTW_CLIENT
#endif

#define NTW_VERSION 42

#define NTW_DEFAULT_TIMEOUT 5.f


/**************************
 * ERRORS MESSAGE / code
 * ***********************/

#define NTW_WELCOM_MSG "hello!"
#define NTW_ERROR_NO 0

#define NTW_ERROR_REQUEST_ADD_MSG "ERROR: to much client connected"
#define NTW_ERROR_REQUEST_ADD 1

#define NTW_ERROR_DISPATCH_ADD_MSG "ERROR: connection to dispatch not etablish"
#define NTW_ERROR_DISPATCH_ADD 2

#endif

#ifndef __SLL_ERROR_H__
#define __SLL_ERROR_H__ 1
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Error
 * \group error
 * \desc Docs!
 */
/**
 * \flags subgroup
 * \name Types
 * \group error
 * \subgroup error-type
 * \desc Docs!
 */



/**
 * \flags macro var
 * \name SLL_ERROR_NO_FILE_PATH
 * \group error
 * \subgroup error-type
 * \desc Docs!
 * \type sll_error_t
 */
#define SLL_ERROR_NO_FILE_PATH 1



/**
 * \flags macro var
 * \name SLL_ERROR_UNKNOWN_FD
 * \group error
 * \subgroup error-type
 * \desc Docs!
 * \type sll_error_t
 */
#define SLL_ERROR_UNKNOWN_FD 2



/**
 * \flags macro var
 * \name SLL_ERROR_SANDBOX
 * \group error
 * \subgroup error-type
 * \desc Docs!
 * \type sll_error_t
 */
#define SLL_ERROR_SANDBOX 3



/**
 * \flags macro var
 * \name SLL_ERROR_TOO_LONG
 * \group error
 * \subgroup error-type
 * \desc Docs!
 * \type sll_error_t
 */
#define SLL_ERROR_TOO_LONG 4



/**
 * \flags macro var
 * \name SLL_ERROR_FLAG_WINAPI
 * \group error
 * \desc Docs!
 * \type sll_error_t
 */
#define SLL_ERROR_FLAG_WINAPI 0x100000000



/**
 * \flags macro var
 * \name SLL_ERROR_FLAG_LIBC
 * \group error
 * \desc Docs!
 * \type sll_error_t
 */
#define SLL_ERROR_FLAG_LIBC 0x200000000



/**
 * \flags func macro
 * \name SLL_ERROR_GET_TYPE
 * \group error
 * \desc Docs!
 * \arg sll_error_t e
 * \ret sll_error_t
 */
#define SLL_ERROR_GET_TYPE(e) ((e)&0x300000000)



/**
 * \flags macro var
 * \name SLL_NO_ERROR
 * \group error
 * \desc Docs!
 * \type sll_error_t
 */
#define SLL_NO_ERROR 0xffffffffffffffffull



#endif

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
 * \name SLL_ERROR_EOF
 * \group error
 * \subgroup error-type
 * \desc Docs!
 * \type sll_error_t
 */
#define SLL_ERROR_EOF 5



/**
 * \flags macro var
 * \name SLL_UNMAPPED_WINDOWS_ERROR
 * \group error
 * \desc Docs!
 * \type sll_error_t
 */
#define SLL_UNMAPPED_WINDOWS_ERROR 255



/**
 * \flags macro var
 * \name SLL_ERROR_FLAG_SLL
 * \group error
 * \desc Docs!
 * \type sll_error_t
 */
#define SLL_ERROR_FLAG_SLL 0



/**
 * \flags macro var
 * \name SLL_ERROR_FLAG_SYSTEM
 * \group error
 * \desc Docs!
 * \type sll_error_t
 */
#define SLL_ERROR_FLAG_SYSTEM 0x100



/**
 * \flags func macro
 * \name SLL_ERROR_GET_TYPE
 * \group error
 * \desc Docs!
 * \arg sll_error_t e
 * \ret sll_error_t
 */
#define SLL_ERROR_GET_TYPE(e) ((e)&0x100)



/**
 * \flags func macro
 * \name SLL_ERROR_GET_VALUE
 * \group error
 * \desc Docs!
 * \arg sll_error_t e
 * \ret sll_error_t
 */
#define SLL_ERROR_GET_VALUE(e) ((e)&0xff)



/**
 * \flags func macro
 * \name SLL_ERROR_GET_EXTRA
 * \group error
 * \desc Docs!
 * \arg sll_error_t e
 * \ret sll_error_t
 */
#define SLL_ERROR_GET_EXTRA(e) ((e)>>9)



/**
 * \flags func macro
 * \name SLL_ERROR_FROM_SANDBOX
 * \group error
 * \desc Docs!
 * \arg sll_error_t e
 * \ret sll_error_t
 */
#define SLL_ERROR_FROM_SANDBOX(f) (((f)<<9)|SLL_ERROR_SANDBOX)



/**
 * \flags macro var
 * \name SLL_NO_ERROR
 * \group error
 * \desc Docs!
 * \type sll_error_t
 */
#define SLL_NO_ERROR 0xffffffffffffffffull



#endif

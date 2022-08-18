#ifndef __SLL_ERROR_H__
#define __SLL_ERROR_H__ 1
#include <sll/common.h>
#include <sll/string.h>
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
 * \name SLL_ERROR_BASE64_PADDING
 * \group error
 * \subgroup error-type
 * \desc Docs!
 * \type sll_error_t
 */
#define SLL_ERROR_BASE64_PADDING 6



/**
 * \flags macro var
 * \name SLL_ERROR_BASE64_CHARACTER
 * \group error
 * \subgroup error-type
 * \desc Docs!
 * \type sll_error_t
 */
#define SLL_ERROR_BASE64_CHARACTER 7



/**
 * \flags macro var
 * \name SLL_ERROR_STRING
 * \group error
 * \subgroup error-type
 * \desc Docs!
 * \type sll_error_t
 */
#define SLL_ERROR_STRING 8



/**
 * \flags macro var
 * \name SLL_ERROR_UNKNOWN_FUNCTION
 * \group error
 * \subgroup error-type
 * \desc Docs!
 * \type sll_error_t
 */
#define SLL_ERROR_UNKNOWN_FUNCTION 9



/**
 * \flags macro var
 * \name SLL_ERROR_NOT_A_SOCKET
 * \group error
 * \subgroup error-type
 * \desc Docs!
 * \type sll_error_t
 */
#define SLL_ERROR_NOT_A_SOCKET 10



/**
 * \flags macro var
 * \name SLL_ERROR_INITIALIZATION
 * \group error
 * \subgroup error-type
 * \desc Docs!
 * \type sll_error_t
 */
#define SLL_ERROR_INITIALIZATION 11



/**
 * \flags macro var
 * \name SLL_UNMAPPED_SYSTEM_ERROR
 * \group error
 * \desc Docs!
 * \type sll_error_t
 */
#define SLL_UNMAPPED_SYSTEM_ERROR 255



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
 * \arg sll_error_t error
 * \ret sll_error_t
 */
#define SLL_ERROR_GET_TYPE(error) ((error)&0x100)



/**
 * \flags func macro
 * \name SLL_ERROR_GET_VALUE
 * \group error
 * \desc Docs!
 * \arg sll_error_t error
 * \ret sll_error_t
 */
#define SLL_ERROR_GET_VALUE(error) ((error)&0xff)



/**
 * \flags func macro
 * \name SLL_ERROR_GET_EXTRA
 * \group error
 * \desc Docs!
 * \arg sll_error_t error
 * \ret sll_error_t
 */
#define SLL_ERROR_GET_EXTRA(error) ((error)>>9)



/**
 * \flags func macro
 * \name SLL_ERROR_FROM_EXTRA
 * \group error
 * \desc Docs!
 * \arg sll_error_t type
 * \arg sll_error_t extra
 * \ret sll_error_t
 */
#define SLL_ERROR_FROM_EXTRA(type,extra) ((((sll_error_t)(extra))<<9)|(type))



/**
 * \flags func macro
 * \name SLL_ERROR_FROM_SANDBOX
 * \group error
 * \desc Docs!
 * \arg sll_error_t flag
 * \ret sll_error_t
 */
#define SLL_ERROR_FROM_SANDBOX(flag) SLL_ERROR_FROM_EXTRA(SLL_ERROR_SANDBOX,(flag))



/**
 * \flags func macro
 * \name SLL_ERROR_FROM_STRING_POINTER
 * \group error
 * \desc Docs!
 * \arg void* pointer
 * \ret sll_error_t
 */
#define SLL_ERROR_FROM_STRING_POINTER(pointer) SLL_ERROR_FROM_EXTRA(SLL_ERROR_STRING,(pointer))



/**
 * \flags macro var
 * \name SLL_NO_ERROR
 * \group error
 * \desc Docs!
 * \type sll_error_t
 */
#define SLL_NO_ERROR 0xffffffffffffffffull



/**
 * \flags type var
 * \name sll_error_t
 * \group error
 * \desc Docs!
 * \type __SLL_U64
 */
typedef __SLL_U64 sll_error_t;



/**
 * \flags check_output func
 * \name sll_error_from_string_pointer
 * \group error
 * \desc Docs!
 * \arg const sll_char_t* string
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_error_from_string_pointer(const sll_char_t* string);



/**
 * \flags check_output func
 * \name sll_error_get_string_pointer
 * \group error
 * \desc Docs!
 * \arg sll_error_t error
 * \ret const sll_char_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT const sll_char_t* sll_error_get_string_pointer(sll_error_t error);



/**
 * \flags func
 * \name sll_error_raise
 * \group error
 * \desc Docs!
 * \arg sll_error_t error
 */
__SLL_EXTERNAL void sll_error_raise(sll_error_t error);



#endif

#ifndef __SLL_API_INT_H__
#define __SLL_API_INT_H__ 1
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Integer API
 * \group int-api
 * \desc Docs!
 */



/**
 * \flags func
 * \name sll_api_int_to_bin
 * \group int-api
 * \desc Docs!
 * \api i|s
 * \arg sll_integer_t a
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_int_to_bin(sll_integer_t a,sll_string_t* out);



/**
 * \flags func
 * \name sll_api_int_to_dec
 * \group int-api
 * \desc Docs!
 * \api i|s
 * \arg sll_integer_t a
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_int_to_dec(sll_integer_t a,sll_string_t* out)



/**
 * \flags func
 * \name sll_api_int_to_hex
 * \group int-api
 * \desc Docs!
 * \api i|s
 * \arg sll_integer_t a
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_int_to_hex(sll_integer_t a,sll_string_t* out);



/**
 * \flags func
 * \name sll_api_int_to_oct
 * \group int-api
 * \desc Docs!
 * \api i|s
 * \arg sll_integer_t a
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_int_to_oct(sll_integer_t a,sll_string_t* out);



#endif

#ifndef __SLL_API_SERIAL_H__
#define __SLL_API_SERIAL_H__ 1
#include <sll/api.h>
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Serialization API
 * \group serial-api
 * \desc Docs!
 */



/**
 * \flags check_output func
 * \name sll_decode_object
 * \group serial-api
 * \desc Docs!
 * \arg sll_file_t* f
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_decode_object(sll_file_t* f);



/**
 * \flags func
 * \name sll_decode_string
 * \group serial-api
 * \desc Docs!
 * \arg sll_file_t* f
 * \arg sll_string_t* o
 * \ret sll_bool_t
 */
__SLL_EXTERNAL sll_bool_t sll_decode_string(sll_file_t* f,sll_string_t* o);



/**
 * \flags check_output func
 * \name sll_encode_object
 * \group serial-api
 * \desc Docs!
 * \arg sll_file_t* f
 * \arg sll_object_t*const* a
 * \arg sll_arg_count_t ac
 */
__SLL_EXTERNAL void sll_encode_object(sll_file_t* f,sll_object_t*const* a,sll_arg_count_t ac);



/**
 * \flags func
 * \name sll_encode_string
 * \group serial-api
 * \desc Docs!
 * \arg sll_file_t* f
 * \arg const sll_string_t* s
 */
__SLL_EXTERNAL void sll_encode_string(sll_file_t* f,const sll_string_t* s);



/**
 * \flags api func
 * \name sll_api_serial_decode_object
 * \group serial-api
 * \desc Docs!
 * \arg I
 * \ret O
 */
__API_FUNC_DECL(serial_decode_object);



/**
 * \flags api func
 * \name sll_api_serial_decode_string
 * \group serial-api
 * \desc Docs!
 * \arg I
 * \ret S
 */
__API_FUNC_DECL(serial_decode_string);



/**
 * \flags api func var_arg
 * \name sll_api_serial_encode_object
 * \group serial-api
 * \desc Docs!
 * \arg I
 * \arg O
 */
__API_FUNC_DECL(serial_encode_object);



/**
 * \flags api func
 * \name sll_api_serial_encode_string
 * \group serial-api
 * \desc Docs!
 * \arg I
 * \arg S
 */
__API_FUNC_DECL(serial_encode_string);



#endif

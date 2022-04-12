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
 * \flags subgroup
 * \name Compression
 * \group serial-api
 * \subgroup serial-api-compression
 * \desc Docs!
 */



/**
 * \flags func macro
 * \name SLL_DECODE_SIGNED_INTEGER
 * \group serial-api
 * \subgroup serial-api-compression
 * \desc Docs!
 * \arg sll_size_t v
 * \ret sll_integer_t
 */
#define SLL_DECODE_SIGNED_INTEGER(v) (((v)>>1)^(-((sll_integer_t)((v)&1))))



/**
 * \flags func macro
 * \name SLL_ENCODE_SIGNED_INTEGER
 * \group serial-api
 * \subgroup serial-api-compression
 * \desc Docs!
 * \arg sll_integer_t v
 * \ret sll_size_t
 */
#define SLL_ENCODE_SIGNED_INTEGER(v) ((((v)<0?~(v):(v))<<1)|((v)<0))



/**
 * \flags check_output func
 * \name sll_decode_integer
 * \group serial-api
 * \subgroup serial-api-compression
 * \desc Docs!
 * \arg sll_file_t* f
 * \arg sll_error_t* err
 * \ret sll_size_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_size_t sll_decode_integer(sll_file_t* f,sll_error_t* err);



/**
 * \flags check_output func
 * \name sll_decode_signed_integer
 * \group serial-api
 * \subgroup serial-api-compression
 * \desc Docs!
 * \arg sll_file_t* f
 * \arg sll_error_t* err
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_integer_t sll_decode_signed_integer(sll_file_t* f,sll_error_t* err);



/**
 * \flags check_output func
 * \name sll_decode_object
 * \group serial-api
 * \subgroup serial-api-compression
 * \desc Docs!
 * \arg sll_file_t* f
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_decode_object(sll_file_t* f);



/**
 * \flags check_output func
 * \name sll_decode_string
 * \group serial-api
 * \subgroup serial-api-compression
 * \desc Docs!
 * \arg sll_file_t* f
 * \arg sll_string_t* o
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_decode_string(sll_file_t* f,sll_string_t* o);



/**
 * \flags func
 * \name sll_encode_integer
 * \group serial-api
 * \subgroup serial-api-compression
 * \desc Docs!
 * \arg sll_file_t* f
 * \arg sll_size_t v
 */
__SLL_EXTERNAL void sll_encode_integer(sll_file_t* f,sll_size_t v);



/**
 * \flags func
 * \name sll_encode_signed_integer
 * \group serial-api
 * \subgroup serial-api-compression
 * \desc Docs!
 * \arg sll_file_t* f
 * \arg sll_integer_t v
 */
__SLL_EXTERNAL void sll_encode_signed_integer(sll_file_t* f,sll_integer_t v);



/**
 * \flags func
 * \name sll_encode_object
 * \group serial-api
 * \subgroup serial-api-compression
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
 * \subgroup serial-api-compression
 * \desc Docs!
 * \arg sll_file_t* f
 * \arg const sll_string_t* s
 */
__SLL_EXTERNAL void sll_encode_string(sll_file_t* f,const sll_string_t* s);



/**
 * \flags api func
 * \name sll_api_serial_decode_float
 * \group serial-api
 * \desc Docs!
 * \arg I
 * \ret F
 */
__API_FUNC_DECL(serial_decode_float);



/**
 * \flags api func
 * \name sll_api_serial_decode_integer
 * \group serial-api
 * \desc Docs!
 * \arg I
 * \ret I
 */
__API_FUNC_DECL(serial_decode_integer);



/**
 * \flags api func
 * \name sll_api_serial_decode_signed_integer
 * \group serial-api
 * \desc Docs!
 * \arg I
 * \ret I
 */
__API_FUNC_DECL(serial_decode_signed_integer);



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
 * \ret O
 */
__API_FUNC_DECL(serial_decode_string);



/**
 * \flags api func
 * \name sll_api_serial_encode_float
 * \group serial-api
 * \desc Docs!
 * \arg I
 * \arg F
 */
__API_FUNC_DECL(serial_encode_float);



/**
 * \flags api func
 * \name sll_api_serial_encode_integer
 * \group serial-api
 * \desc Docs!
 * \arg I
 * \arg I
 */
__API_FUNC_DECL(serial_encode_integer);



/**
 * \flags api func
 * \name sll_api_serial_encode_signed_integer
 * \group serial-api
 * \desc Docs!
 * \arg I
 * \arg I
 */
__API_FUNC_DECL(serial_encode_signed_integer);



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

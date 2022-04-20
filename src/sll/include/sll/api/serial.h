#ifndef __SLL_API_SERIAL_H__
#define __SLL_API_SERIAL_H__ 1
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
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_decode_string(sll_file_t* f,sll_string_t* o);



/**
 * \flags check_output func
 * \name sll_encode_integer
 * \group serial-api
 * \subgroup serial-api-compression
 * \desc Docs!
 * \arg sll_file_t* f
 * \arg sll_size_t v
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_encode_integer(sll_file_t* f,sll_size_t v);



/**
 * \flags check_output func
 * \name sll_encode_signed_integer
 * \group serial-api
 * \subgroup serial-api-compression
 * \desc Docs!
 * \arg sll_file_t* f
 * \arg sll_integer_t v
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_encode_signed_integer(sll_file_t* f,sll_integer_t v);



/**
 * \flags check_output func
 * \name sll_encode_object
 * \group serial-api
 * \subgroup serial-api-compression
 * \desc Docs!
 * \arg sll_file_t* f
 * \arg sll_object_t*const* a
 * \arg sll_arg_count_t ac
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_encode_object(sll_file_t* f,sll_object_t*const* a,sll_arg_count_t ac);



/**
 * \flags check_output func
 * \name sll_encode_string
 * \group serial-api
 * \subgroup serial-api-compression
 * \desc Docs!
 * \arg sll_file_t* f
 * \arg const sll_string_t* s
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_encode_string(sll_file_t* f,const sll_string_t* s);



/**
 * \flags check_output func
 * \name sll_api_serial_decode_float
 * \group serial-api
 * \desc Docs!
 * \api i|~f
 * \arg sll_integer_t fh
 * \arg sll_float_t* out
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_serial_decode_float(sll_integer_t fh,sll_float_t* out);



/**
 * \flags check_output func
 * \name sll_api_serial_decode_integer
 * \group serial-api
 * \desc Docs!
 * \api i|Q
 * \arg sll_integer_t fh
 * \ret sll_size_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_size_t sll_api_serial_decode_integer(sll_integer_t fh);



/**
 * \flags check_output func
 * \name sll_api_serial_decode_signed_integer
 * \group serial-api
 * \desc Docs!
 * \api i|~a
 * \arg sll_integer_t fh
 * \arg sll_array_t* out
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_serial_decode_signed_integer(sll_integer_t fh,sll_array_t* out);



/**
 * \flags check_output func
 * \name sll_api_serial_decode_object
 * \group serial-api
 * \desc Docs!
 * \api i|o
 * \arg sll_integer_t fh
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_object_t* sll_api_serial_decode_object(sll_integer_t fh);



/**
 * \flags check_output func
 * \name sll_api_serial_decode_string
 * \group serial-api
 * \desc Docs!
 * \api i|~s
 * \arg sll_integer_t fh
 * \arg sll_string_t* out
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_serial_decode_string(sll_integer_t fh,sll_string_t* out);



/**
 * \flags func
 * \name sll_api_serial_encode_float
 * \group serial-api
 * \desc Docs!
 * \api if|Q
 * \arg sll_integer_t fh
 * \arg sll_float_t v
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_serial_encode_float(sll_integer_t fh,sll_float_t v);



/**
 * \flags func
 * \name sll_api_serial_encode_integer
 * \group serial-api
 * \desc Docs!
 * \api iQ|Q
 * \arg sll_integer_t fh
 * \arg sll_size_t v
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_serial_encode_integer(sll_integer_t fh,sll_size_t v);



/**
 * \flags func
 * \name sll_api_serial_encode_signed_integer
 * \group serial-api
 * \desc Docs!
 * \api ii|Q
 * \arg sll_integer_t fh
 * \arg sll_integer_t v
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_serial_encode_signed_integer(sll_integer_t fh,sll_integer_t v);



/**
 * \flags func
 * \name sll_api_serial_encode_object
 * \group serial-api
 * \desc Docs!
 * \api io!|Q
 * \arg sll_integer_t fh
 * \arg sll_object_t*const* args
 * \arg sll_arg_count_t len
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_serial_encode_object(sll_integer_t fh,sll_object_t*const* args,sll_arg_count_t len);



/**
 * \flags func
 * \name sll_api_serial_encode_string
 * \group serial-api
 * \desc Docs!
 * \api i#s|Q
 * \arg sll_integer_t fh
 * \arg const sll_string_t* str
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_serial_encode_string(sll_integer_t fh,const sll_string_t* str);



#endif

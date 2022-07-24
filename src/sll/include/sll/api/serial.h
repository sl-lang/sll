#ifndef __SLL_API_SERIAL_H__
#define __SLL_API_SERIAL_H__ 1
#include <sll/api/file.h>
#include <sll/common.h>
#include <sll/error.h>
#include <sll/file.h>
#include <sll/gc.h>
#include <sll/string.h>
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
 * \name sll_api_serial_decode_float
 * \group serial-api
 * \desc Docs!
 * \api D|~f
 * \arg sll_file_handle_t hande
 * \arg sll_float_t* out
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_serial_decode_float(sll_file_handle_t hande,sll_float_t* out);



/**
 * \flags check_output func
 * \name sll_api_serial_decode_integer
 * \group serial-api
 * \desc Docs!
 * \api D|~a
 * \arg sll_file_handle_t hande
 * \arg sll_array_t* out
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_serial_decode_integer(sll_file_handle_t hande,sll_array_t* out);



/**
 * \flags check_output func
 * \name sll_api_serial_decode_object
 * \group serial-api
 * \desc Docs!
 * \api D|~a
 * \arg sll_file_handle_t hande
 * \arg sll_array_t* out
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_serial_decode_object(sll_file_handle_t hande,sll_array_t* out);



/**
 * \flags check_output func
 * \name sll_api_serial_decode_signed_integer
 * \group serial-api
 * \desc Docs!
 * \api D|~a
 * \arg sll_file_handle_t hande
 * \arg sll_array_t* out
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_serial_decode_signed_integer(sll_file_handle_t hande,sll_array_t* out);



/**
 * \flags check_output func
 * \name sll_api_serial_decode_string
 * \group serial-api
 * \desc Docs!
 * \api D|~s
 * \arg sll_file_handle_t hande
 * \arg sll_string_t* out
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_serial_decode_string(sll_file_handle_t hande,sll_string_t* out);



/**
 * \flags func
 * \name sll_api_serial_encode_float
 * \group serial-api
 * \desc Docs!
 * \api if|Q
 * \arg sll_file_handle_t hande
 * \arg sll_float_t v
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_serial_encode_float(sll_file_handle_t hande,sll_float_t v);



/**
 * \flags func
 * \name sll_api_serial_encode_integer
 * \group serial-api
 * \desc Docs!
 * \api DQ|Q
 * \arg sll_file_handle_t hande
 * \arg sll_size_t v
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_serial_encode_integer(sll_file_handle_t hande,sll_size_t v);



/**
 * \flags func
 * \name sll_api_serial_encode_object
 * \group serial-api
 * \desc Docs!
 * \api D#a|Q
 * \arg sll_file_handle_t hande
 * \arg const sll_array_t* args
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_serial_encode_object(sll_file_handle_t hande,const sll_array_t* args);



/**
 * \flags func
 * \name sll_api_serial_encode_signed_integer
 * \group serial-api
 * \desc Docs!
 * \api Di|Q
 * \arg sll_file_handle_t hande
 * \arg sll_integer_t v
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_serial_encode_signed_integer(sll_file_handle_t hande,sll_integer_t v);



/**
 * \flags func
 * \name sll_api_serial_encode_string
 * \group serial-api
 * \desc Docs!
 * \api D#s|Q
 * \arg sll_file_handle_t hande
 * \arg const sll_string_t* str
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_serial_encode_string(sll_file_handle_t hande,const sll_string_t* str);



/**
 * \flags check_output func
 * \name sll_decode_integer
 * \group serial-api
 * \subgroup serial-api-compression
 * \desc Docs!
 * \arg sll_file_t* file
 * \arg sll_error_t* err
 * \ret sll_size_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_size_t sll_decode_integer(sll_file_t* file,sll_error_t* err);



/**
 * \flags check_output func
 * \name sll_decode_signed_integer
 * \group serial-api
 * \subgroup serial-api-compression
 * \desc Docs!
 * \arg sll_file_t* file
 * \arg sll_error_t* err
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_integer_t sll_decode_signed_integer(sll_file_t* file,sll_error_t* err);



/**
 * \flags check_output func
 * \name sll_decode_object
 * \group serial-api
 * \subgroup serial-api-compression
 * \desc Docs!
 * \arg sll_file_t* file
 * \arg sll_error_t* err
 * \ret sll_object_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t sll_decode_object(sll_file_t* file,sll_error_t* err);



/**
 * \flags check_output func
 * \name sll_decode_string
 * \group serial-api
 * \subgroup serial-api-compression
 * \desc Docs!
 * \arg sll_file_t* file
 * \arg sll_string_t* out
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_decode_string(sll_file_t* file,sll_string_t* out);



/**
 * \flags check_output func
 * \name sll_encode_integer
 * \group serial-api
 * \subgroup serial-api-compression
 * \desc Docs!
 * \arg sll_file_t* file
 * \arg sll_size_t v
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_encode_integer(sll_file_t* file,sll_size_t v);



/**
 * \flags check_output func
 * \name sll_encode_signed_integer
 * \group serial-api
 * \subgroup serial-api-compression
 * \desc Docs!
 * \arg sll_file_t* file
 * \arg sll_integer_t v
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_encode_signed_integer(sll_file_t* file,sll_integer_t v);



/**
 * \flags check_output func
 * \name sll_encode_object
 * \group serial-api
 * \subgroup serial-api-compression
 * \desc Docs!
 * \arg sll_file_t* file
 * \arg const sll_object_t* args
 * \arg sll_arg_count_t arg_count
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_encode_object(sll_file_t* file,const sll_object_t* args,sll_arg_count_t arg_count);



/**
 * \flags check_output func
 * \name sll_encode_string
 * \group serial-api
 * \subgroup serial-api-compression
 * \desc Docs!
 * \arg sll_file_t* file
 * \arg const sll_string_t* string
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_encode_string(sll_file_t* file,const sll_string_t* string);



#endif

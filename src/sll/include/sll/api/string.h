#ifndef __SLL_API_STRING_H__
#define __SLL_API_STRING_H__ 1
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/parse_args.h>
#include <sll/types.h>
/**
 * \flags group
 * \name String API
 * \group string-api
 * \desc Docs!
 */



/**
 * \flags check_output func
 * \name sll_api_string_checksum
 * \group string-api
 * \desc Docs!
 * \api #s|D
 * \arg const sll_string_t* str
 * \ret sll_string_checksum_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_string_checksum_t sll_api_string_checksum(const sll_string_t* str);



/**
 * \flags func
 * \name sll_api_string_convert
 * \group string-api
 * \desc Docs!
 * \api o!|s
 * \arg sll_object_t*const* args
 * \arg sll_arg_count_t len
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_convert(sll_object_t*const* args,sll_arg_count_t len,sll_string_t* out);



/**
 * \flags check_output func
 * \name sll_api_string_count
 * \group string-api
 * \desc Docs!
 * \api #sy|i
 * \arg const sll_string_t* str
 * \arg const sll_char_string_t* elem
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_string_count(const sll_string_t* str,const sll_char_string_t* elem);



/**
 * \flags check_output func
 * \name sll_api_string_count_left
 * \group string-api
 * \desc Docs!
 * \api #sc|i
 * \arg const sll_string_t* str
 * \arg sll_char_t chr
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_string_count_left(const sll_string_t* str,sll_char_t chr);



/**
 * \flags check_output func
 * \name sll_api_string_count_right
 * \group string-api
 * \desc Docs!
 * \api #sc|i
 * \arg const sll_string_t* str
 * \arg sll_char_t chr
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_string_count_right(const sll_string_t* str,sll_char_t chr);



/**
 * \flags check_output func
 * \name sll_api_string_ends
 * \group string-api
 * \desc Docs!
 * \api #sy|b
 * \arg const sll_string_t* str
 * \arg const sll_char_string_t* end
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_string_ends(const sll_string_t* str,const sll_char_string_t* end);



/**
 * \flags func
 * \name sll_api_string_format
 * \group string-api
 * \desc Docs!
 * \api #so!|s
 * \arg const sll_string_t* fmt
 * \arg sll_object_t*const* args
 * \arg sll_arg_count_t len
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_format(const sll_string_t* fmt,sll_object_t*const* args,sll_arg_count_t len,sll_string_t* out);



/**
 * \flags func
 * \name sll_api_string_flip_case
 * \group string-api
 * \desc Docs!
 * \api #s|s
 * \arg const sll_string_t* str
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_flip_case(const sll_string_t* str,sll_string_t* out);



/**
 * \flags check_output func
 * \name sll_api_string_index
 * \group string-api
 * \desc Docs!
 * \api #syD|i
 * \arg const sll_string_t* str
 * \arg const sll_char_string_t* substr
 * \arg sll_string_length_t start
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_string_index(const sll_string_t* str,const sll_char_string_t* substr,sll_string_length_t start);



/**
 * \flags check_output func
 * \name sll_api_string_index_list
 * \group string-api
 * \desc Docs!
 * \api #syb|i
 * \arg const sll_string_t* str
 * \arg const sll_char_string_t* substr
 * \arg sll_bool_t inv
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_string_index_list(const sll_string_t* str,const sll_char_string_t* substr,sll_bool_t inv);



/**
 * \flags check_output func
 * \name sll_api_string_index_reverse
 * \group string-api
 * \desc Docs!
 * \api #sy|i
 * \arg const sll_string_t* str
 * \arg const sll_char_string_t* substr
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_string_index_reverse(const sll_string_t* str,const sll_char_string_t* substr);



/**
 * \flags check_output func
 * \name sll_api_string_index_reverse_list
 * \group string-api
 * \desc Docs!
 * \api #syb|i
 * \arg const sll_string_t* str
 * \arg const sll_char_string_t* substr
 * \arg sll_bool_t inv
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_string_index_reverse_list(const sll_string_t* str,const sll_char_string_t* substr,sll_bool_t inv);



/**
 * \flags func
 * \name sll_api_string_join
 * \group string-api
 * \desc Docs!
 * \api y#a|s
 * \arg const sll_char_string_t* infix
 * \arg const sll_array_t* arr
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_join(const sll_char_string_t* infix,const sll_array_t* arr,sll_string_t* out);



/**
 * \flags func
 * \name sll_api_string_pad
 * \group string-api
 * \desc Docs!
 * \api #sDc|s
 * \arg const sll_string_t* str
 * \arg sll_string_length_t len
 * \arg sll_char_t chr
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_pad(const sll_string_t* str,sll_string_length_t len,sll_char_t chr,sll_string_t* out);



/**
 * \flags func
 * \name sll_api_string_pad_left
 * \group string-api
 * \desc Docs!
 * \api #sDc|s
 * \arg const sll_string_t* str
 * \arg sll_string_length_t len
 * \arg sll_char_t chr
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_pad_left(const sll_string_t* str,sll_string_length_t len,sll_char_t chr,sll_string_t* out);



/**
 * \flags func
 * \name sll_api_string_pad_right
 * \group string-api
 * \desc Docs!
 * \api #sDc|s
 * \arg const sll_string_t* str
 * \arg sll_string_length_t len
 * \arg sll_char_t chr
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_pad_right(const sll_string_t* str,sll_string_length_t len,sll_char_t chr,sll_string_t* out);



/**
 * \flags func
 * \name sll_api_string_replace
 * \group string-api
 * \desc Docs!
 * \api #syy|s
 * \arg const sll_string_t* str
 * \arg const sll_char_string_t* old
 * \arg const sll_char_string_t* new
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_replace(const sll_string_t* str,const sll_char_string_t* old,const sll_char_string_t* new,sll_string_t* out);



/**
 * \flags func
 * \name sll_api_string_reverse
 * \group string-api
 * \desc Docs!
 * \api #s|s
 * \arg const sll_string_t* str
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_reverse(const sll_string_t* str,sll_string_t* out);



/**
 * \flags check_output func
 * \name sll_api_string_secure_equal
 * \group string-api
 * \desc Docs!
 * \api #s#s|b
 * \arg const sll_string_t* a
 * \arg const sll_string_t* b
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_string_secure_equal(const sll_string_t* a,const sll_string_t* b);



/**
 * \flags func
 * \name sll_api_string_split
 * \group string-api
 * \desc Docs!
 * \api #sy|a
 * \arg const sll_string_t* str
 * \arg const sll_char_string_t* infix
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_split(const sll_string_t* str,const sll_char_string_t* infix,sll_array_t* out);



/**
 * \flags check_output func
 * \name sll_api_string_starts
 * \group string-api
 * \desc Docs!
 * \api #sy|b
 * \arg const sll_string_t* str
 * \arg const sll_char_string_t* start
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_string_starts(const sll_string_t* str,const sll_char_string_t* start);



/**
 * \flags func
 * \name sll_api_string_to_lower_case
 * \group string-api
 * \desc Docs!
 * \api #s|s
 * \arg const sll_string_t* str
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_to_lower_case(const sll_string_t* str,sll_string_t* out);



/**
 * \flags func
 * \name sll_api_string_to_title_case
 * \group string-api
 * \desc Docs!
 * \api #s|s
 * \arg const sll_string_t* str
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_to_title_case(const sll_string_t* str,sll_string_t* out);



/**
 * \flags func
 * \name sll_api_string_trim
 * \group string-api
 * \desc Docs!
 * \api #s|s
 * \arg const sll_string_t* str
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_trim(const sll_string_t* str,sll_string_t* out);



/**
 * \flags func
 * \name sll_api_string_trim_left
 * \group string-api
 * \desc Docs!
 * \api #s|s
 * \arg const sll_string_t* str
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_trim_left(const sll_string_t* str,sll_string_t* out);



/**
 * \flags func
 * \name sll_api_string_trim_right
 * \group string-api
 * \desc Docs!
 * \api #s|s
 * \arg const sll_string_t* str
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_trim_right(const sll_string_t* str,sll_string_t* out);



/**
 * \flags func
 * \name sll_api_string_to_upper_case
 * \group string-api
 * \desc Docs!
 * \api #s|s
 * \arg const sll_string_t* str
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_to_upper_case(const sll_string_t* str,sll_string_t* out);



#endif

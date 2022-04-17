#ifndef __SLL_API_STRING_H__
#define __SLL_API_STRING_H__ 1
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name String API
 * \group string-api
 * \desc Docs!
 */



/**
 * \flags func
 * \name sll_api_string_checksum
 * \group string-api
 * \desc Docs!
 * \api s|i
 * \arg sll_string_t* a
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_string_checksum(sll_string_t* a);



/**
 * \flags func
 * \name sll_api_string_convert
 * \group string-api
 * \desc Docs!
 * \api o!|s
 * \arg sll_object_t*const* a
 * \arg sll_arg_count_t ac
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_convert(sll_object_t*const* a,sll_arg_count_t ac,sll_string_t* out);



/**
 * \flags func
 * \name sll_api_string_count
 * \group string-api
 * \desc Docs!
 * \api sy|i
 * \arg sll_string_t* a
 * \arg sll_char_string_t* b
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_string_count(sll_string_t* a,sll_char_string_t* b);



/**
 * \flags func
 * \name sll_api_string_count_left
 * \group string-api
 * \desc Docs!
 * \api sc|i
 * \arg sll_string_t* a
 * \arg sll_char_t b
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_string_count_left(sll_string_t* a,sll_char_t b);



/**
 * \flags func
 * \name sll_api_string_count_right
 * \group string-api
 * \desc Docs!
 * \api sc|i
 * \arg sll_string_t* a
 * \arg sll_char_t b
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_string_count_right(sll_string_t* a,sll_char_t b);



/**
 * \flags func
 * \name sll_api_string_ends
 * \group string-api
 * \desc Docs!
 * \api sy|b
 * \arg sll_string_t* a
 * \arg sll_char_string_t* b
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_string_ends(sll_string_t* a,sll_char_string_t* b);



/**
 * \flags func
 * \name sll_api_string_format
 * \group string-api
 * \desc Docs!
 * \api so!|s
 * \arg sll_string_t* a
 * \arg sll_object_t*const* b
 * \arg sll_arg_count_t bc
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_format(sll_string_t* a,sll_object_t*const* b,sll_arg_count_t bc,sll_string_t* out);



/**
 * \flags func
 * \name sll_api_string_flip_case
 * \group string-api
 * \desc Docs!
 * \api s|s
 * \arg sll_string_t* a
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_flip_case(sll_string_t* a,sll_string_t* out);



/**
 * \flags func
 * \name sll_api_string_index
 * \group string-api
 * \desc Docs!
 * \api syi|i
 * \arg sll_string_t* a
 * \arg sll_char_string_t* b
 * \arg sll_integer_t c
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_string_index(sll_string_t* a,sll_char_string_t* b,sll_integer_t c);



/**
 * \flags func
 * \name sll_api_string_index_list
 * \group string-api
 * \desc Docs!
 * \api syb|i
 * \arg sll_string_t* a
 * \arg sll_char_string_t* b
 * \arg sll_bool_t c
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_string_index_list(sll_string_t* a,sll_char_string_t* b,sll_bool_t c);



/**
 * \flags func
 * \name sll_api_string_index_reverse
 * \group string-api
 * \desc Docs!
 * \api sy|i
 * \arg sll_string_t* a
 * \arg sll_char_string_t* b
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_string_index_reverse(sll_string_t* a,sll_char_string_t* b);



/**
 * \flags func
 * \name sll_api_string_index_reverse_list
 * \group string-api
 * \desc Docs!
 * \api syb|i
 * \arg sll_string_t* a
 * \arg sll_char_string_t* b
 * \arg sll_bool_t c
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_string_index_reverse_list(sll_string_t* a,sll_char_string_t* b,sll_bool_t c);



/**
 * \flags func
 * \name sll_api_string_join
 * \group string-api
 * \desc Docs!
 * \api ya|s
 * \arg sll_char_string_t* a
 * \arg sll_array_t* b
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_join(sll_char_string_t* a,sll_array_t* b,sll_string_t* out);



/**
 * \flags func
 * \name sll_api_string_pad
 * \group string-api
 * \desc Docs!
 * \api sic|s
 * \arg sll_string_t* a
 * \arg sll_integer_t b
 * \arg sll_char_t c
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_pad(sll_string_t* a,sll_integer_t b,sll_char_t c,sll_string_t* out);



/**
 * \flags func
 * \name sll_api_string_pad_left
 * \group string-api
 * \desc Docs!
 * \api sic|s
 * \arg sll_string_t* a
 * \arg sll_integer_t b
 * \arg sll_char_t c
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_pad_left(sll_string_t* a,sll_integer_t b,sll_char_t c,sll_string_t* out);



/**
 * \flags func
 * \name sll_api_string_pad_right
 * \group string-api
 * \desc Docs!
 * \api sic|s
 * \arg sll_string_t* a
 * \arg sll_integer_t b
 * \arg sll_char_t c
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_pad_right(sll_string_t* a,sll_integer_t b,sll_char_t c,sll_string_t* out);



/**
 * \flags func
 * \name sll_api_string_replace
 * \group string-api
 * \desc Docs!
 * \api syy|s
 * \arg sll_string_t* a
 * \arg sll_char_string_t* b
 * \arg sll_char_string_t* c
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_replace(sll_string_t* a,sll_char_string_t* b,sll_char_string_t* c,sll_string_t* out);



/**
 * \flags func
 * \name sll_api_string_reverse
 * \group string-api
 * \desc Docs!
 * \api s|s
 * \arg sll_string_t* a
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_reverse(sll_string_t* a,sll_string_t* out);



/**
 * \flags func
 * \name sll_api_string_secure_equal
 * \group string-api
 * \desc Docs!
 * \api ss|b
 * \arg sll_string_t* a
 * \arg sll_string_t* b
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_string_secure_equal(sll_string_t* a,sll_string_t* b);



/**
 * \flags func
 * \name sll_api_string_split
 * \group string-api
 * \desc Docs!
 * \api sy|a
 * \arg sll_string_t* a
 * \arg sll_char_string_t* b
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_split(sll_string_t* a,sll_char_string_t* b,sll_array_t* out);



/**
 * \flags func
 * \name sll_api_string_starts
 * \group string-api
 * \desc Docs!
 * \api sy|b
 * \arg sll_string_t* a
 * \arg sll_char_string_t* b
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_string_starts(sll_string_t* a,sll_char_string_t* b);



/**
 * \flags func
 * \name sll_api_string_to_lower_case
 * \group string-api
 * \desc Docs!
 * \api s|s
 * \arg sll_string_t* a
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_to_lower_case(sll_string_t* a,sll_string_t* out);



/**
 * \flags func
 * \name sll_api_string_to_title_case
 * \group string-api
 * \desc Docs!
 * \api s|s
 * \arg sll_string_t* a
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_to_title_case(sll_string_t* a,sll_string_t* out);



/**
 * \flags func
 * \name sll_api_string_trim
 * \group string-api
 * \desc Docs!
 * \api s|s
 * \arg sll_string_t* a
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_trim(sll_string_t* a,sll_string_t* out);



/**
 * \flags func
 * \name sll_api_string_trim_left
 * \group string-api
 * \desc Docs!
 * \api s|s
 * \arg sll_string_t* a
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_trim_left(sll_string_t* a,sll_string_t* out);



/**
 * \flags func
 * \name sll_api_string_trim_right
 * \group string-api
 * \desc Docs!
 * \api s|s
 * \arg sll_string_t* a
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_trim_right(sll_string_t* a,sll_string_t* out);



/**
 * \flags func
 * \name sll_api_string_to_upper_case
 * \group string-api
 * \desc Docs!
 * \api s|s
 * \arg sll_string_t* a
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_string_to_upper_case(sll_string_t* a,sll_string_t* out);



#endif

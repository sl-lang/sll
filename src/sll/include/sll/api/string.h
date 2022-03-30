#ifndef __SLL_API_STRING_H__
#define __SLL_API_STRING_H__ 1
#include <sll/api.h>
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name String API
 * \group string-api
 * \desc Docs!
 */



/**
 * \flags api func
 * \name sll_api_string_checksum
 * \group string-api
 * \desc Docs!
 * \arg S
 * \ret I
 */
__API_FUNC_DECL(string_checksum);



/**
 * \flags api func var_arg
 * \name sll_api_string_convert
 * \group string-api
 * \desc Docs!
 * \arg O
 * \ret S
 */
__API_FUNC_DECL(string_convert);



/**
 * \flags api func
 * \name sll_api_string_count
 * \group string-api
 * \desc Docs!
 * \arg S
 * \arg CS
 * \ret I
 */
__API_FUNC_DECL(string_count);



/**
 * \flags api func
 * \name sll_api_string_count_left
 * \group string-api
 * \desc Docs!
 * \arg S
 * \arg C
 * \ret I
 */
__API_FUNC_DECL(string_count_left);



/**
 * \flags api func
 * \name sll_api_string_count_right
 * \group string-api
 * \desc Docs!
 * \arg S
 * \arg C
 * \ret I
 */
__API_FUNC_DECL(string_count_right);



/**
 * \flags api func
 * \name sll_api_string_ends
 * \group string-api
 * \desc Docs!
 * \arg S
 * \arg CS
 * \ret B
 */
__API_FUNC_DECL(string_ends);



/**
 * \flags api func var_arg
 * \name sll_api_string_format
 * \group string-api
 * \desc Docs!
 * \arg S
 * \arg O
 * \ret S
 */
__API_FUNC_DECL(string_format);



/**
 * \flags api func
 * \name sll_api_string_flip_case
 * \group string-api
 * \desc Docs!
 * \arg S
 * \ret S
 */
__API_FUNC_DECL(string_flip_case);



/**
 * \flags api func
 * \name sll_api_string_index
 * \group string-api
 * \desc Docs!
 * \arg S
 * \arg CS
 * \arg I
 * \ret I
 */
__API_FUNC_DECL(string_index);



/**
 * \flags api func
 * \name sll_api_string_index_list
 * \group string-api
 * \desc Docs!
 * \arg S
 * \arg CS
 * \arg B
 * \ret I
 */
__API_FUNC_DECL(string_index_list);



/**
 * \flags api func
 * \name sll_api_string_index_reverse
 * \group string-api
 * \desc Docs!
 * \arg S
 * \arg CS
 * \ret I
 */
__API_FUNC_DECL(string_index_reverse);



/**
 * \flags api func
 * \name sll_api_string_index_reverse_list
 * \group string-api
 * \desc Docs!
 * \arg S
 * \arg CS
 * \arg B
 * \ret I
 */
__API_FUNC_DECL(string_index_reverse_list);



/**
 * \flags api func
 * \name sll_api_string_join
 * \group string-api
 * \desc Docs!
 * \arg CS
 * \arg A
 * \ret S
 */
__API_FUNC_DECL(string_join);



/**
 * \flags api func
 * \name sll_api_string_pad
 * \group string-api
 * \desc Docs!
 * \arg S
 * \arg I
 * \arg C
 * \ret S
 */
__API_FUNC_DECL(string_pad);



/**
 * \flags api func
 * \name sll_api_string_pad_left
 * \group string-api
 * \desc Docs!
 * \arg S
 * \arg I
 * \arg C
 * \ret S
 */
__API_FUNC_DECL(string_pad_left);



/**
 * \flags api func
 * \name sll_api_string_pad_right
 * \group string-api
 * \desc Docs!
 * \arg S
 * \arg I
 * \arg C
 * \ret S
 */
__API_FUNC_DECL(string_pad_right);



/**
 * \flags api func
 * \name sll_api_string_replace
 * \group string-api
 * \desc Docs!
 * \arg S
 * \arg CS
 * \arg CS
 * \ret S
 */
__API_FUNC_DECL(string_replace);



/**
 * \flags api func
 * \name sll_api_string_reverse
 * \group string-api
 * \desc Docs!
 * \arg S
 * \ret S
 */
__API_FUNC_DECL(string_reverse);



/**
 * \flags api func
 * \name sll_api_string_secure_equal
 * \group string-api
 * \desc Docs!
 * \arg S
 * \arg S
 * \ret B
 */
__API_FUNC_DECL(string_secure_equal);



/**
 * \flags api func
 * \name sll_api_string_split
 * \group string-api
 * \desc Docs!
 * \arg S
 * \arg CS
 * \ret A
 */
__API_FUNC_DECL(string_split);



/**
 * \flags api func
 * \name sll_api_string_starts
 * \group string-api
 * \desc Docs!
 * \arg S
 * \arg CS
 * \ret B
 */
__API_FUNC_DECL(string_starts);



/**
 * \flags api func
 * \name sll_api_string_to_lower_case
 * \group string-api
 * \desc Docs!
 * \arg S
 * \ret S
 */
__API_FUNC_DECL(string_to_lower_case);



/**
 * \flags api func
 * \name sll_api_string_to_title_case
 * \group string-api
 * \desc Docs!
 * \arg S
 * \ret S
 */
__API_FUNC_DECL(string_to_title_case);



/**
 * \flags api func
 * \name sll_api_string_trim
 * \group string-api
 * \desc Docs!
 * \arg S
 * \ret S
 */
__API_FUNC_DECL(string_trim);



/**
 * \flags api func
 * \name sll_api_string_trim_left
 * \group string-api
 * \desc Docs!
 * \arg S
 * \ret S
 */
__API_FUNC_DECL(string_trim_left);



/**
 * \flags api func
 * \name sll_api_string_trim_right
 * \group string-api
 * \desc Docs!
 * \arg S
 * \ret S
 */
__API_FUNC_DECL(string_trim_right);



/**
 * \flags api func
 * \name sll_api_string_to_upper_case
 * \group string-api
 * \desc Docs!
 * \arg S
 * \ret S
 */
__API_FUNC_DECL(string_to_upper_case);



#endif

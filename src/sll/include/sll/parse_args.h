#ifndef __SLL_PARSE_ARGS_H__
#define __SLL_PARSE_ARGS_H__ 1
#include <sll/common.h>
#include <sll/types.h>
#include <stdarg.h>
/**
 * \flags group
 * \name Argument Parsing
 * \group parse-args
 * \desc Docs!
 */



/**
 * \flags macro var
 * \name SLL_PARSE_ARGS_TYPE_INT
 * \group parse-args
 * \desc Docs!
 * \type sll_bool_t
 */
#define SLL_PARSE_ARGS_TYPE_INT 0



/**
 * \flags macro var
 * \name SLL_PARSE_ARGS_TYPE_FLOAT
 * \group parse-args
 * \desc Docs!
 * \type sll_bool_t
 */
#define SLL_PARSE_ARGS_TYPE_FLOAT 1



/**
 * \flags macro var
 * \name SLL_PARSE_ARGS_TYPE_CHAR
 * \group parse-args
 * \desc Docs!
 * \type sll_bool_t
 */
#define SLL_PARSE_ARGS_TYPE_CHAR 0



/**
 * \flags macro var
 * \name SLL_PARSE_ARGS_TYPE_STRING
 * \group parse-args
 * \desc Docs!
 * \type sll_bool_t
 */
#define SLL_PARSE_ARGS_TYPE_STRING 1



/**
 * \flags func
 * \name sll_free_args
 * \group parse-args
 * \desc Docs!
 * \arg sll_arg_state_t dt
 */
__SLL_EXTERNAL void sll_free_args(sll_arg_state_t dt);



/**
 * \flags check_output func
 * \name sll_parse_arg_count
 * \group parse-args
 * \desc Docs!
 * \arg const sll_char_t* t
 * \ret sll_arg_count_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_arg_count_t sll_parse_arg_count(const sll_char_t* t);



/**
 * \flags check_output func var_arg
 * \name sll_parse_args
 * \group parse-args
 * \desc Docs!
 * \arg const sll_char_t* t
 * \arg sll_object_t*const* al
 * \arg sll_arg_count_t all
 * \ret sll_arg_state_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_arg_state_t sll_parse_args(const sll_char_t* t,sll_object_t*const* al,sll_arg_count_t all,...);



/**
 * \flags check_output func
 * \name sll_parse_args_list
 * \group parse-args
 * \desc Docs!
 * \arg const sll_char_t* t
 * \arg sll_object_t*const* al
 * \arg sll_arg_count_t all
 * \arg va_list* va
 * \ret sll_arg_state_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_arg_state_t sll_parse_args_list(const sll_char_t* t,sll_object_t*const* al,sll_arg_count_t all,va_list* va);



#endif

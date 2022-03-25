#ifndef __SLL_PARSE_ARGS_H__
#define __SLL_PARSE_ARGS_H__ 1
#include <sll/common.h>
#include <sll/types.h>
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
 * \flags func var_arg
 * \name sll_parse_args
 * \group parse-args
 * \desc Docs!
 * \arg const sll_char_t* t
 * \arg sll_object_t*const* al
 * \arg sll_arg_count_t all
 */
__SLL_EXTERNAL void sll_parse_args(const sll_char_t* t,sll_object_t*const* al,sll_arg_count_t all,...);



#endif

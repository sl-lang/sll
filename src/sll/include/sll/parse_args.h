#ifndef __SLL_PARSE_ARGS_H__
#define __SLL_PARSE_ARGS_H__ 1
#include <sll/_size_types.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/string.h>
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
 * \type sll_number_type_t
 */
#define SLL_PARSE_ARGS_TYPE_INT 0



/**
 * \flags macro var
 * \name SLL_PARSE_ARGS_TYPE_FLOAT
 * \group parse-args
 * \desc Docs!
 * \type sll_number_type_t
 */
#define SLL_PARSE_ARGS_TYPE_FLOAT 1



/**
 * \flags macro var
 * \name SLL_PARSE_ARGS_TYPE_COMPLEX
 * \group parse-args
 * \desc Docs!
 * \type sll_number_type_t
 */
#define SLL_PARSE_ARGS_TYPE_COMPLEX 2



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
 * \flags type var
 * \name sll_number_type_t
 * \group parse-args
 * \desc Docs!
 * \type __SLL_U8
 */
typedef __SLL_U8 sll_number_type_t;



/**
 * \flags type var
 * \name sll_arg_state_t
 * \group parse-args
 * \desc Docs!
 * \type void*
 */
typedef void* sll_arg_state_t;



/**
 * \flags type union
 * \name sll_number_data_t
 * \group parse-args
 * \desc Docs!
 * \arg sll_integer_t i
 * \arg sll_float_t f
 * \arg sll_complex_t d
 */
typedef union _SLL_NUMBER_DATA{
    sll_integer_t i;
	sll_float_t f;
	sll_complex_t d;
} sll_number_data_t;



/**
 * \flags type
 * \name sll_number_t
 * \group parse-args
 * \desc Docs!
 * \arg sll_number_type_t t
 * \arg sll_number_data_t dt
 */
typedef struct _SLL_NUMBER{
	sll_number_type_t t;
	sll_number_data_t dt;
} sll_number_t;



/**
 * \flags type union
 * \name sll_char_string_data_t
 * \group parse-args
 * \desc Docs!
 * \arg sll_char_t c
 * \arg const sll_string_t* s
 */
typedef union _SLL_CHAR_STRING_DATA{
	sll_char_t c;
	const sll_string_t* s;
} sll_char_string_data_t;



/**
 * \flags type
 * \name sll_char_string_t
 * \group parse-args
 * \desc Docs!
 * \arg sll_bool_t t
 * \arg sll_char_string_data_t dt
 */
typedef struct _SLL_CHAR_STRING{
	sll_bool_t t;
	sll_char_string_data_t dt;
} sll_char_string_t;



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

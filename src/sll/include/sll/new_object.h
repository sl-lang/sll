#ifndef __SLL_NEW_OBJECT_H__
#define __SLL_NEW_OBJECT_H__ 1
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/string.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Object Creation
 * \group new-object
 * \desc Docs!
 */



/**
 * \flags func macro
 * \name SLL_OFFSETOF
 * \group new-object
 * \desc Docs!
 * \arg __type__ type
 * \arg __field__ field
 * \ret sll_size_t
 */
#define SLL_OFFSETOF(type,field) ((sll_size_t)(&(((type*)NULL)->field)))



/**
 * \flags check_output func var_arg
 * \name sll_new_object
 * \group new-object
 * \desc Docs!
 * \arg const sll_char_t* format
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_new_object(const sll_char_t* format,...);



/**
 * \flags func var_arg
 * \name sll_new_object_array
 * \group new-object
 * \desc Docs!
 * \arg const sll_char_t* format
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL void sll_new_object_array(const sll_char_t* format,sll_array_t* out,...);



/**
 * \flags func
 * \name sll_new_object_array_list
 * \group new-object
 * \desc Docs!
 * \arg const sll_char_t* format
 * \arg sll_string_length_t format_length
 * \arg sll_var_arg_list_t* va
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL void sll_new_object_array_list(const sll_char_t* format,sll_string_length_t format_length,sll_var_arg_list_t* va,sll_array_t* out);



/**
 * \flags check_output func
 * \name sll_new_object_list
 * \group new-object
 * \desc Docs!
 * \arg const sll_char_t* format
 * \arg sll_string_length_t format_length
 * \arg sll_var_arg_list_t* va
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_new_object_list(const sll_char_t* format,sll_string_length_t format_length,sll_var_arg_list_t* va);



#endif

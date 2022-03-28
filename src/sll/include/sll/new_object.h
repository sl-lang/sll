#ifndef __SLL_NEW_OBJECT_H__
#define __SLL_NEW_OBJECT_H__ 1
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Object Creation
 * \group new-object
 * \desc Docs!
 */



/**
 * \flags check_output func var_arg
 * \name sll_new_object
 * \group new-object
 * \desc Docs!
 * \arg const sll_char_t* t
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_new_object(const sll_char_t* t,...);



/**
 * \flags func var_arg
 * \name sll_new_object_array
 * \group new-object
 * \desc Docs!
 * \arg const sll_char_t* t
 * \arg sll_array_t* o
 */
__SLL_EXTERNAL void sll_new_object_array(const sll_char_t* t,sll_array_t* o,...);



/**
 * \flags func
 * \name sll_new_object_list
 * \group new-object
 * \desc Docs!
 * \arg const sll_char_t* t
 * \arg sll_string_length_t tl
 * \arg sll_var_arg_list_t* va
 * \arg sll_array_t* o
 */
__SLL_EXTERNAL void sll_new_object_array_list(const sll_char_t* t,sll_string_length_t tl,sll_var_arg_list_t* va,sll_array_t* o);



/**
 * \flags check_output func
 * \name sll_new_object_list
 * \group new-object
 * \desc Docs!
 * \arg const sll_char_t* t
 * \arg sll_string_length_t tl
 * \arg sll_var_arg_list_t* va
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_new_object_list(const sll_char_t* t,sll_string_length_t tl,sll_var_arg_list_t* va);



#endif

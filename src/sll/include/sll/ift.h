#ifndef __SLL_IFT_H__
#define __SLL_IFT_H__ 1
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Internal Function Table
 * \group ift
 * \desc Docs!
 */



/**
 * \flags macro var
 * \name SLL_MAX_FUNCTION_INDEX
 * \group identifier
 * \desc Docs!
 * \type sll_function_index_t
 */
#define SLL_MAX_FUNCTION_INDEX 0xfffffffe



/**
 * \flags macro var
 * \name SLL_UNKNOWN_INTERNAL_FUNCTION_INDEX
 * \group identifier
 * \desc Docs!
 * \type sll_function_index_t
 */
#define SLL_UNKNOWN_INTERNAL_FUNCTION_INDEX 0xffffffff



/**
 * \flags func
 * \name sll_clone_internal_function_table
 * \group ift
 * \desc Docs!
 * \arg sll_internal_function_table_t* ift
 * \arg sll_internal_function_table_t* o
 */
__SLL_EXTERNAL void sll_clone_internal_function_table(sll_internal_function_table_t* ift,sll_internal_function_table_t* o);



/**
 * \flags func
 * \name sll_create_internal_function_table
 * \group ift
 * \desc Docs!
 * \arg sll_internal_function_table_t* o
 */
__SLL_EXTERNAL void sll_create_internal_function_table(sll_internal_function_table_t* o);



/**
 * \flags func
 * \name sll_free_internal_function_table
 * \group ift
 * \desc Docs!
 * \arg sll_internal_function_table_t* ift
 */
__SLL_EXTERNAL void sll_free_internal_function_table(sll_internal_function_table_t* ift);



/**
 * \flags check_output func
 * \name sll_lookup_internal_function
 * \group ift
 * \desc Docs!
 * \arg const sll_internal_function_table_t* i_ft
 * \arg const sll_char_t* nm
 * \ret sll_function_index_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_function_index_t sll_lookup_internal_function(const sll_internal_function_table_t* i_ft,const sll_char_t* nm);



/**
 * \flags func
 * \name sll_register_builtin_internal_functions
 * \group ift
 * \desc Docs!
 * \arg sll_internal_function_table_t* ift
 */
__SLL_EXTERNAL void sll_register_builtin_internal_functions(sll_internal_function_table_t* ift);



/**
 * \flags func
 * \name sll_register_internal_function
 * \group ift
 * \desc Docs!
 * \arg sll_internal_function_table_t* i_ft
 * \arg const sll_char_t* nm
 * \arg sll_internal_function_pointer_t f
 * \ret sll_function_index_t
 */
__SLL_EXTERNAL sll_function_index_t sll_register_internal_function(sll_internal_function_table_t* i_ft,const sll_char_t* nm,sll_internal_function_pointer_t f);



#endif

#ifndef __SLL_IFT_H__
#define __SLL_IFT_H__ 1
#include <sll/_size_types.h>
#include <sll/common.h>
#include <sll/string.h>
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
 * \group ift
 * \desc Docs!
 * \type sll_function_index_t
 */
#define SLL_MAX_FUNCTION_INDEX 0xfffffffe



/**
 * \flags macro var
 * \name SLL_UNKNOWN_INTERNAL_FUNCTION_INDEX
 * \group ift
 * \desc Docs!
 * \type sll_function_index_t
 */
#define SLL_UNKNOWN_INTERNAL_FUNCTION_INDEX 0xffffffff



/**
 * \flags type var
 * \name sll_internal_function_pointer_t
 * \group ift
 * \desc Docs!
 * \type void*
 */
typedef void* sll_internal_function_pointer_t;



/**
 * \flags type
 * \name sll_internal_function_t
 * \group ift
 * \desc Docs!
 * \arg sll_string_t nm
 * \arg sll_internal_function_pointer_t p
 * \arg sll_char_t* fmt
 * \arg __SLL_U16 _ret
 * \arg sll_arg_count_t _arg_cnt
 * \arg sll_size_t _arg_sz
 * \arg __SLL_U64* _regs
 */
typedef struct _SLL_INTERNAL_FUNCTION{
	sll_string_t nm;
	sll_internal_function_pointer_t p;
	sll_char_t* fmt;
	__SLL_U16 _ret;
	sll_arg_count_t _arg_cnt;
	sll_size_t _arg_sz;
	__SLL_U64* _regs;
} sll_internal_function_t;



/**
 * \flags type
 * \name sll_internal_function_table_t
 * \group ift
 * \desc Docs!
 * \arg const sll_internal_function_t* dt
 * \arg sll_function_index_t l
 */
typedef struct _SLL_INTERNAL_FUNCTION_TABLE{
	const sll_internal_function_t* dt;
	sll_function_index_t l;
} sll_internal_function_table_t;



/**
 * \flags type
 * \name sll_internal_function_descriptor_t
 * \group ift
 * \desc Docs!
 * \arg const sll_char_t* nm
 * \arg const sll_internal_function_pointer_t f
 * \arg const sll_char_t* fmt
 */
typedef struct _SLL_INTERNAL_FUNCTION_DESCRIPTOR{
	const sll_char_t* nm;
	const sll_internal_function_pointer_t f;
	const sll_char_t* fmt;
} sll_internal_function_descriptor_t;



/**
 * \flags var
 * \name sll_builtin_internal_function_data
 * \group ift
 * \desc Docs!
 * \type const sll_internal_function_descriptor_t*
 */
__SLL_EXTERNAL extern const sll_internal_function_descriptor_t* sll_builtin_internal_function_data;



/**
 * \flags var
 * \name sll_builtin_internal_function_count
 * \group ift
 * \desc Docs!
 * \type const sll_function_index_t
 */
__SLL_EXTERNAL extern const sll_function_index_t sll_builtin_internal_function_count;



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
 * \name sll_register_internal_function
 * \group ift
 * \desc Docs!
 * \arg sll_internal_function_table_t* i_ft
 * \arg const sll_char_t* nm
 * \arg const sll_char_t* fmt
 * \arg sll_internal_function_pointer_t f
 * \ret sll_function_index_t
 */
__SLL_EXTERNAL sll_function_index_t sll_register_internal_function(sll_internal_function_table_t* i_ft,const sll_char_t* nm,const sll_char_t* fmt,sll_internal_function_pointer_t f);



/**
 * \flags func
 * \name sll_register_internal_functionss
 * \group ift
 * \desc Docs!
 * \arg sll_internal_function_table_t* i_ft
 * \arg const sll_internal_function_descriptor_t* dt
 * \arg sll_function_index_t len
 */
__SLL_EXTERNAL void sll_register_internal_functions(sll_internal_function_table_t* i_ft,const sll_internal_function_descriptor_t* dt,sll_function_index_t len);



#endif

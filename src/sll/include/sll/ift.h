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
 * \arg sll_string_t name
 * \arg sll_internal_function_pointer_t function
 * \arg sll_char_t* format
 * \arg __SLL_U16 _return_value
 * \arg sll_arg_count_t _arg_count
 * \arg sll_size_t _arg_size
 * \arg __SLL_U64* _registers
 */
typedef struct _SLL_INTERNAL_FUNCTION{
	sll_string_t name;
	sll_internal_function_pointer_t function;
	sll_char_t* format;
	__SLL_U16 _return_value;
	sll_arg_count_t _arg_count;
	sll_size_t _arg_size;
	__SLL_U64* _registers;
} sll_internal_function_t;



/**
 * \flags type
 * \name sll_internal_function_table_t
 * \group ift
 * \desc Docs!
 * \arg const sll_internal_function_t* data
 * \arg sll_function_index_t length
 */
typedef struct _SLL_INTERNAL_FUNCTION_TABLE{
	const sll_internal_function_t* data;
	sll_function_index_t length;
} sll_internal_function_table_t;



/**
 * \flags type
 * \name sll_internal_function_descriptor_t
 * \group ift
 * \desc Docs!
 * \arg const sll_char_t* name
 * \arg const sll_internal_function_pointer_t function
 * \arg const sll_char_t* format
 */
typedef struct _SLL_INTERNAL_FUNCTION_DESCRIPTOR{
	const sll_char_t* name;
	const sll_internal_function_pointer_t function;
	const sll_char_t* format;
} sll_internal_function_descriptor_t;



/**
 * \flags type
 * \name sll_internal_function_table_descriptor_t
 * \group ift
 * \desc Docs!
 * \arg const sll_internal_function_descriptor_t* data
 * \arg sll_function_index_t length
 */
typedef struct _SLL_INTERNAL_FUNCTION_TABLE_DESCRIPTOR{
   const sll_internal_function_descriptor_t* data;
    sll_function_index_t length;
} sll_internal_function_table_descriptor_t;



/**
 * \flags var
 * \name sll_builtin_internal_functions
 * \group ift
 * \desc Docs!
 * \type const sll_internal_function_table_descriptor_t*
 */
__SLL_EXTERNAL extern const sll_internal_function_table_descriptor_t* sll_builtin_internal_functions;



/**
 * \flags func
 * \name sll_clone_internal_function_table
 * \group ift
 * \desc Docs!
 * \arg sll_internal_function_table_t* internal_function_table
 * \arg sll_internal_function_table_t* out
 */
__SLL_EXTERNAL void sll_clone_internal_function_table(sll_internal_function_table_t* internal_function_table,sll_internal_function_table_t* out);



/**
 * \flags func
 * \name sll_create_internal_function_table
 * \group ift
 * \desc Docs!
 * \arg sll_internal_function_table_t* out
 */
__SLL_EXTERNAL void sll_create_internal_function_table(sll_internal_function_table_t* out);



/**
 * \flags func
 * \name sll_free_internal_function_table
 * \group ift
 * \desc Docs!
 * \arg sll_internal_function_table_t* internal_function_table
 */
__SLL_EXTERNAL void sll_free_internal_function_table(sll_internal_function_table_t* internal_function_table);



/**
 * \flags check_output func
 * \name sll_lookup_internal_function
 * \group ift
 * \desc Docs!
 * \arg const sll_internal_function_table_t* internal_function_table
 * \arg const sll_char_t* name
 * \ret sll_function_index_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_function_index_t sll_lookup_internal_function(const sll_internal_function_table_t* internal_function_table,const sll_char_t* name);



/**
 * \flags func
 * \name sll_register_internal_function
 * \group ift
 * \desc Docs!
 * \arg sll_internal_function_table_t* internal_function_table
 * \arg const sll_char_t* name
 * \arg const sll_char_t* format
 * \arg sll_internal_function_pointer_t function
 * \ret sll_function_index_t
 */
__SLL_EXTERNAL sll_function_index_t sll_register_internal_function(sll_internal_function_table_t* internal_function_table,const sll_char_t* name,const sll_char_t* format,sll_internal_function_pointer_t function);



/**
 * \flags func
 * \name sll_register_internal_functions
 * \group ift
 * \desc Docs!
 * \arg sll_internal_function_table_t* internal_function_table
 * \arg const sll_internal_function_descriptor_t* data
 * \arg sll_function_index_t length
 */
__SLL_EXTERNAL void sll_register_internal_functions(sll_internal_function_table_t* internal_function_table,const sll_internal_function_descriptor_t* data,sll_function_index_t length);



#endif

#ifndef __SLL_VAR_ARG_H__
#define __SLL_VAR_ARG_H__ 1
#include <sll/_string_types.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/types.h>
#include <stdarg.h>
/**
 * \flags group
 * \name Variable Arguments
 * \group var-arg
 * \desc Docs!
 */



/**
 * \flags macro var
 * \name SLL_VAR_ARG_LIST_TYPE_C
 * \group var-arg
 * \desc Docs!
 * \type sll_var_arg_list_type_t
 */
#define SLL_VAR_ARG_LIST_TYPE_C 0



/**
 * \flags macro var
 * \name SLL_VAR_ARG_LIST_TYPE_SLL
 * \group var-arg
 * \desc Docs!
 * \type sll_var_arg_list_type_t
 */
#define SLL_VAR_ARG_LIST_TYPE_SLL 1



/**
 * \flags macro var
 * \name SLL_VAR_ARG_LIST_TYPE_RESERVED0
 * \group var-arg
 * \desc Internally used as `VAR_ARG_LIST_TYPE_STRUCT`
 * \type sll_var_arg_list_type_t
 */
#define SLL_VAR_ARG_LIST_TYPE_RESERVED0 2



/**
 * \flags func macro
 * \name SLL_VAR_ARG_INIT_C
 * \group var-arg
 * \desc Docs!
 * \arg sll_var_arg_list_t* va
 * \arg va_list* va_ptr
 */
#define SLL_VAR_ARG_INIT_C(va,va_ptr) \
	do{ \
		sll_var_arg_list_t* __va=(va); \
		__va->type=SLL_VAR_ARG_LIST_TYPE_C; \
		__va->data.c=(va_ptr); \
	} while (0)



/**
 * \flags func macro
 * \name SLL_VAR_ARG_INIT_SLL
 * \group var-arg
 * \desc Docs!
 * \arg sll_var_arg_list_t* va
 * \arg sll_object_t*const* ptr
 * \arg sll_arg_count_t cnt
 */
#define SLL_VAR_ARG_INIT_SLL(va,ptr,cnt) \
	do{ \
		sll_var_arg_list_t* __va=(va); \
		__va->type=SLL_VAR_ARG_LIST_TYPE_SLL; \
		__va->data.sll.pointer=(ptr); \
		__va->data.sll.count=(cnt); \
	} while (0)



/**
 * \flags type var
 * \name sll_var_arg_list_type_t
 * \group var-arg
 * \desc Docs!
 * \type __SLL_U8
 */
typedef __SLL_U8 sll_var_arg_list_type_t;



/**
 * \flags type
 * \name sll_var_arg_list_data_sll_t
 * \group var-arg
 * \desc Docs!
 * \arg sll_object_t*const* pointer
 * \arg sll_arg_count_t count
 */
typedef struct _SLL_VAR_ARG_LIST_DATA_SLL{
	sll_object_t*const* pointer;
	sll_arg_count_t count;
} sll_var_arg_list_data_sll_t;



/**
 * \flags type
 * \name sll_var_arg_list_data_struct_t
 * \group var-arg
 * \desc Docs!
 * \arg const void* base_pointer
 * \arg sll_size_t* offset_data
 * \arg sll_arg_count_t offset_count
 * \arg void** converter_function_data
 * \arg sll_arg_count_t converter_function_count
 */
typedef struct _SLL_VAR_ARG_LIST_DATA_STRUCT{
	const void* base_pointer;
	sll_size_t* offset_data;
	sll_arg_count_t offset_count;
	void** converter_function_data;
	sll_arg_count_t converter_function_count;
} sll_var_arg_list_data_struct_t;



/**
 * \flags type union
 * \name sll_var_arg_list_data_t
 * \group var-arg
 * \desc Docs!
 * \arg va_list* c
 * \arg sll_var_arg_list_data_sll_t sll
 * \arg sll_var_arg_list_data_struct_t struct_
 */
typedef union _SLL_VAR_ARG_LIST_DATA{
	va_list* c;
	sll_var_arg_list_data_sll_t sll;
	sll_var_arg_list_data_struct_t struct_;
} sll_var_arg_list_data_t;



/**
 * \flags type
 * \name sll_var_arg_list_t
 * \group var-arg
 * \desc Docs!
 * \arg sll_var_arg_list_type_t type
 * \arg sll_var_arg_list_data_t data
 */
typedef struct _SLL_VAR_ARG_LIST{
	sll_var_arg_list_type_t type;
	sll_var_arg_list_data_t data;
} sll_var_arg_list_t;



/**
 * \flags check_output func
 * \name sll_var_arg_get
 * \group var-arg
 * \desc Docs!
 * \arg sll_var_arg_list_t* va
 * \ret void*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_var_arg_get(sll_var_arg_list_t* va);



/**
 * \flags check_output func
 * \name sll_var_arg_get_char
 * \group var-arg
 * \desc Docs!
 * \arg sll_var_arg_list_t* va
 * \ret sll_char_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_char_t sll_var_arg_get_char(sll_var_arg_list_t* va);



/**
 * \flags func
 * \name sll_var_arg_get_complex
 * \group var-arg
 * \desc Docs!
 * \arg sll_var_arg_list_t* va
 * \arg sll_complex_t* o
 */
__SLL_EXTERNAL void sll_var_arg_get_complex(sll_var_arg_list_t* va,sll_complex_t* o);



/**
 * \flags check_output func
 * \name sll_var_arg_get_float
 * \group var-arg
 * \desc Docs!
 * \arg sll_var_arg_list_t* va
 * \ret sll_float_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_float_t sll_var_arg_get_float(sll_var_arg_list_t* va);



/**
 * \flags check_output func
 * \name sll_var_arg_get_int
 * \group var-arg
 * \desc Docs!
 * \arg sll_var_arg_list_t* va
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_integer_t sll_var_arg_get_int(sll_var_arg_list_t* va);



/**
 * \flags check_output func
 * \name sll_var_arg_get_object
 * \group var-arg
 * \desc Docs!
 * \arg sll_var_arg_list_t* va
 * \arg sll_object_t* out
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_var_arg_get_object(sll_var_arg_list_t* va);



/**
 * \flags func
 * \name sll_var_arg_get_string
 * \group var-arg
 * \desc Docs!
 * \arg sll_var_arg_list_t* va
 * \arg sll_string_t* o
 */
__SLL_EXTERNAL void sll_var_arg_get_string(sll_var_arg_list_t* va,sll_string_t* o);



#endif

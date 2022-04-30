#ifndef __SLL_VAR_ARG_H__
#define __SLL_VAR_ARG_H__ 1
#include <sll/common.h>
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
 * \type sll_var_arg_type_t
 */
#define SLL_VAR_ARG_LIST_TYPE_C 0



/**
 * \flags macro var
 * \name SLL_VAR_ARG_LIST_TYPE_SLL
 * \group var-arg
 * \desc Docs!
 * \type sll_var_arg_type_t
 */
#define SLL_VAR_ARG_LIST_TYPE_SLL 1



/**
 * \flags macro var
 * \name SLL_VAR_ARG_LIST_TYPE_RESERVED0
 * \group var-arg
 * \desc Internally used as `VAR_ARG_LIST_TYPE_STRUCT`
 * \type sll_var_arg_type_t
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
        __va->t=SLL_VAR_ARG_LIST_TYPE_C; \
        __va->dt.c=(va_ptr); \
    } while (0)



/**
 * \flags func macro
 * \name SLL_VAR_ARG_INIT_SLL
 * \group var-arg
 * \desc Docs!
 * \arg sll_var_arg_list_t* va
 * \arg sll_object_t*const* ptr
 * \arg sll_arg_count_t len
 */
#define SLL_VAR_ARG_INIT_SLL(va,ptr,len) \
    do{ \
        sll_var_arg_list_t* __va=(va); \
        __va->t=SLL_VAR_ARG_LIST_TYPE_SLL; \
        __va->dt.sll.p=(ptr); \
        __va->dt.sll.l=(len); \
    } while (0)



/**
 * \flags type var
 * \name sll_var_arg_type_t
 * \group var-arg
 * \desc Docs!
 * \type __SLL_U8
 */
typedef __SLL_U8 sll_var_arg_type_t;



/**
 * \flags type
 * \name sll_var_arg_list_data_sll_t
 * \group var-arg
 * \desc Docs!
 * \arg sll_object_t*const* p
 * \arg sll_arg_count_t l
 */
typedef struct _SLL_VAR_ARG_LIST_DATA_SLL{
    sll_object_t*const* p;
    sll_arg_count_t l;
} sll_var_arg_list_data_sll_t;



/**
 * \flags type
 * \name sll_var_arg_list_data_struct_t
 * \group var-arg
 * \desc Docs!
 * \arg const void* ptr
 * \arg sll_size_t* off
 * \arg sll_arg_count_t l
 * \arg void** fn
 * \arg sll_arg_count_t fnl
 */
typedef struct _SLL_VAR_ARG_LIST_DATA_STRUCT{
    const void* ptr;
    sll_size_t* off;
    sll_arg_count_t l;
    void** fn;
    sll_arg_count_t fnl;
} sll_var_arg_list_data_struct_t;



/**
 * \flags type union
 * \name sll_var_arg_list_data_t
 * \group var-arg
 * \desc Docs!
 * \arg va_list* c
 * \arg sll_var_arg_list_data_sll_t sll
 * \arg sll_var_arg_list_data_struct_t s
 */
typedef union _SLL_VAR_ARG_LIST_DATA{
    va_list* c;
    sll_var_arg_list_data_sll_t sll;
    sll_var_arg_list_data_struct_t s;
} sll_var_arg_list_data_t;



/**
 * \flags type
 * \name sll_var_arg_list_t
 * \group var-arg
 * \desc Docs!
 * \arg sll_var_arg_type_t t
 * \arg sll_var_arg_list_data_t dt
 */
typedef struct _SLL_VAR_ARG_LIST{
    sll_var_arg_type_t t;
    sll_var_arg_list_data_t dt;
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
 * \flags check_output func
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

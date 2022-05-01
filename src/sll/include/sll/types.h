#ifndef __SLL_TYPES_H__
#define __SLL_TYPES_H__ 1
#include <sll/_identifier.h>
#include <sll/_object_func.h>
#include <sll/_size_types.h>
#include <sll/common.h>
/**
 * \flags group
 * \name Limits
 * \group limits
 * \desc Docs!
 */
/**
 * \flags group
 * \name Types
 * \group types
 * \desc Docs!
 */



/**
 * \flags macro var
 * \name SLL_MAX_ARG_COUNT
 * \group limits
 * \desc Docs!
 * \type sll_arg_count_t
 */
#define SLL_MAX_ARG_COUNT __SLL_U32_MAX



/**
 * \flags macro var
 * \name SLL_MAX_EXPORT_TABLE_LENGTH
 * \group limits
 * \desc Docs!
 * \type sll_export_table_length_t
 */
#define SLL_MAX_EXPORT_TABLE_LENGTH __SLL_U32_MAX



/**
 * \flags macro var
 * \name SLL_MAX_SCOPE
 * \group limits
 * \desc Docs!
 * \type sll_scope_t
 */
#define SLL_MAX_SCOPE __SLL_U32_MAX



/**
 * \flags macro var
 * \name SLL_MAX_SIZE
 * \group limits
 * \desc Docs!
 * \type sll_size_t
 */
#define SLL_MAX_SIZE __SLL_U64_MAX



/**
 * \flags macro var
 * \name SLL_MAX_STACK_OFFSET
 * \group limits
 * \desc Docs!
 * \type sll_stack_offset_t
 */
#define SLL_MAX_STACK_OFFSET __SLL_U32_MAX



/**
 * \flags type var
 * \name sll_bool_t
 * \group types
 * \desc Docs!
 * \type __SLL_U1
 */
typedef __SLL_U1 sll_bool_t;



/**
 * \flags type var
 * \name sll_char_t
 * \group types
 * \desc Docs!
 * \type __SLL_U8
 */
typedef __SLL_U8 sll_char_t;



/**
 * \flags type var
 * \name sll_flags_t
 * \group types
 * \desc Docs!
 * \type __SLL_U8
 */
typedef __SLL_U8 sll_flags_t;



/**
 * \flags type var
 * \name sll_cpu_t
 * \group types
 * \desc Docs!
 * \type __SLL_U16
 */
typedef __SLL_U16 sll_cpu_t;



/**
 * \flags type var
 * \name sll_return_code_t
 * \group types
 * \desc Docs!
 * \type __SLL_S32
 */
typedef __SLL_S32 sll_return_code_t;



/**
 * \flags type var
 * \name sll_arg_count_t
 * \group types
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_arg_count_t;



/**
 * \flags type var
 * \name sll_export_table_length_t
 * \group types
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_export_table_length_t;



/**
 * \flags type var
 * \name sll_function_index_t
 * \group types
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_function_index_t;



/**
 * \flags type var
 * \name sll_stack_offset_t
 * \group types
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_stack_offset_t;



/**
 * \flags type var
 * \name sll_integer_t
 * \group types
 * \desc Docs!
 * \type __SLL_S64
 */
typedef __SLL_S64 sll_integer_t;



/**
 * \flags type var
 * \name sll_size_t
 * \group types
 * \desc Docs!
 * \type __SLL_U64
 */
typedef __SLL_U64 sll_size_t;



/**
 * \flags type var
 * \name sll_float_t
 * \group types
 * \desc Docs!
 * \type __SLL_F64
 */
typedef __SLL_F64 sll_float_t;



/**
 * \flags type
 * \name sll_complex_t
 * \group types
 * \desc Docs!
 * \arg sll_float_t real
 * \arg sll_float_t imag
 */
typedef struct _SLL_COMPLEX{
	sll_float_t real;
	sll_float_t imag;
} sll_complex_t;



#endif

#ifndef __SLL_TYPES_H__
#define __SLL_TYPES_H__ 1
#include <sll/_size_types.h>
/**
 * \flags group
 * \name Types
 * \group types
 * \desc Docs!
 */



/**
 * \flags macro var
 * \name SLL_MAX_ARG_COUNT
 * \group types
 * \desc Docs!
 * \type sll_arg_count_t
 */
#define SLL_MAX_ARG_COUNT __SLL_U32_MAX



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
 * \name sll_arg_count_t
 * \group types
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_arg_count_t;



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

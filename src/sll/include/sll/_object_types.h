#ifndef __SLL__OBJECT_TYPES_H__
#define __SLL__OBJECT_TYPES_H__ 1
#include <sll/_string_types.h>
#include <sll/_size_types.h>
#include <sll/types.h>



struct _SLL_OBJECT;



/**
 * \flags type var
 * \name sll_array_length_t
 * \group array
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_array_length_t;



/**
 * \flags type var
 * \name sll_map_length_t
 * \group map
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_map_length_t;



/**
 * \flags type var
 * \name sll_object_type_t
 * \group gc
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_object_type_t;



/**
 * \flags type var
 * \name sll_reference_count_t
 * \group gc
 * \desc Docs!
 * \type __SLL_U64
 */
typedef __SLL_U64 sll_reference_count_t;



/**
 * \flags type var
 * \name sll_object_pool_index_t
 * \group gc
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_object_pool_index_t;



/**
 * \flags type var
 * \name sll_object_flags_t
 * \group gc
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_object_flags_t;



/**
 * \flags type var
 * \name sll_object_extra_data_t
 * \group gc
 * \desc Docs!
 * \type __SLL_U64
 */
typedef __SLL_U64 sll_object_extra_data_t;



/**
 * \flags type
 * \name sll_array_t
 * \group array
 * \desc Docs!
 * \arg sll_array_length_t length
 * \arg struct _SLL_OBJECT** data
 */
typedef struct _SLL_ARRAY{
	sll_array_length_t length;
	struct _SLL_OBJECT** data;
} sll_array_t;



/**
 * \flags type
 * \name sll_map_t
 * \group map
 * \desc Docs!
 * \arg sll_map_length_t length
 * \arg struct _SLL_OBJECT** data
 */
typedef struct _SLL_MAP{
	sll_map_length_t length;
	struct _SLL_OBJECT** data;
} sll_map_t;



/**
 * \flags type union
 * \name sll_object_field_t
 * \group gc
 * \desc Docs!
 * \arg sll_integer_t int_
 * \arg sll_float_t float_
 * \arg sll_char_t char_
 * \arg struct _SLL_OBJECT* any
 */
typedef union _SLL_OBJECT_FIELD{
	sll_integer_t int_;
	sll_float_t float_;
	sll_char_t char_;
	struct _SLL_OBJECT* any;
} sll_object_field_t;



/**
 * \flags type
 * \name sll_object_ptr_t
 * \group gc
 * \desc Docs!
 * \arg struct _SLL_OBJECT* prev
 * \arg struct _SLL_OBJECT* next
 */
typedef struct _SLL_OBJECT_PTR{
	struct _SLL_OBJECT* prev;
	struct _SLL_OBJECT* next;
} sll_object_ptr_t;



/**
 * \flags type union
 * \name sll_object_data_t
 * \group gc
 * \desc Docs!
 * \arg sll_integer_t int_
 * \arg sll_float_t float_
 * \arg sll_char_t char_
 * \arg sll_complex_t complex_
 * \arg sll_string_t string
 * \arg sll_array_t array
 * \arg sll_map_t map
 * \arg sll_object_field_t* fields
 * \arg sll_object_ptr_t _next_object
 * \arg sll_object_pool_index_t _pool_index
 */
typedef union _SLL_OBJECT_DATA{
	sll_integer_t int_;
	sll_float_t float_;
	sll_char_t char_;
	sll_complex_t complex_;
	sll_string_t string;
	sll_array_t array;
	sll_map_t map;
	sll_object_field_t* fields;
	sll_object_ptr_t _next_object;
	sll_object_pool_index_t _pool_index;
} sll_object_data_t;



/**
 * \flags type
 * \name sll_object_t
 * \group gc
 * \desc Docs!
 * \arg sll_reference_count_t rc
 * \arg const sll_object_type_t type
 * \arg sll_object_flags_t _flags
 * \arg sll_object_extra_data_t _data
 * \arg sll_object_data_t data
 */
typedef struct _SLL_OBJECT{
	sll_reference_count_t rc;
	const sll_object_type_t type;
	sll_object_flags_t _flags;
	sll_object_extra_data_t _data;
	sll_object_data_t data;
} sll_object_t;



#endif

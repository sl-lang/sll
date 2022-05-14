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
 * \flags type
 * \name sll_array_t
 * \group array
 * \desc Docs!
 * \arg sll_array_length_t l
 * \arg sll_object_t** v
 */
typedef struct _SLL_ARRAY{
	sll_array_length_t l;
	struct _SLL_OBJECT** v;
} sll_array_t;



/**
 * \flags type
 * \name sll_map_t
 * \group map
 * \desc Docs!
 * \arg sll_map_length_t l
 * \arg sll_object_t** v
 */
typedef struct _SLL_MAP{
	sll_map_length_t l;
	struct _SLL_OBJECT** v;
} sll_map_t;



/**
 * \flags type union
 * \name sll_object_field_t
 * \group gc
 * \desc Docs!
 * \arg sll_integer_t i
 * \arg sll_float_t f
 * \arg sll_char_t c
 * \arg sll_object_t* o
 */
typedef union _SLL_OBJECT_FIELD{
	sll_integer_t i;
	sll_float_t f;
	sll_char_t c;
	struct _SLL_OBJECT* o;
} sll_object_field_t;



/**
 * \flags type
 * \name sll_object_ptr_t
 * \group gc
 * \desc Docs!
 * \arg sll_object_t* p
 * \arg sll_object_t* n
 */
typedef struct _SLL_OBJECT_PTR{
	struct _SLL_OBJECT* p;
	struct _SLL_OBJECT* n;
} sll_object_ptr_t;



/**
 * \flags type union
 * \name sll_object_data_t
 * \group gc
 * \desc Docs!
 * \arg sll_integer_t i
 * \arg sll_float_t f
 * \arg sll_char_t c
 * \arg sll_complex_t d
 * \arg sll_string_t s
 * \arg sll_array_t a
 * \arg sll_map_t m
 * \arg sll_object_field_t* p
 * \arg sll_object_ptr_t _ptr
 * \arg sll_array_length_t _idx
 */
typedef union _SLL_OBJECT_DATA{
	sll_integer_t i;
	sll_float_t f;
	sll_char_t c;
	sll_complex_t d;
	sll_string_t s;
	sll_array_t a;
	sll_map_t m;
	sll_object_field_t* p;
	sll_object_ptr_t _ptr;
    sll_array_length_t _idx;
} sll_object_data_t;



/**
 * \flags type
 * \name sll_object_t
 * \group gc
 * \desc Docs!
 * \arg sll_reference_count_t rc
 * \arg const sll_object_type_t t
 * \arg __SLL_U32 _f
 * \arg sll_object_data_t dt
 */
typedef struct _SLL_OBJECT{
	sll_reference_count_t rc;
	const sll_object_type_t t;
	__SLL_U32 _f;
	sll_object_data_t dt;
} sll_object_t;



#endif

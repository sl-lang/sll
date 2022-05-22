#ifndef __SLL_ARRAY_H__
#define __SLL_ARRAY_H__ 1
#include <sll/_size_types.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/gc.h>
#include <sll/operator.h>
#include <sll/string.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Array
 * \group array
 * \desc Docs!
 */
/**
 * \flags subgroup
 * \name Content
 * \group array
 * \subgroup array-data
 * \desc Docs!
 */
/**
 * \flags subgroup
 * \name Operations
 * \group array
 * \subgroup array-op
 * \desc Docs!
 */



/**
 * \flags macro var
 * \name SLL_MAX_ARRAY_LENGTH
 * \group array
 * \desc Docs!
 * \type sll_array_length_t
 */
#define SLL_MAX_ARRAY_LENGTH __SLL_U32_MAX



/**
 * \flags func macro
 * \name SLL_INIT_ARRAY
 * \group array
 * \desc Docs!
 * \arg sll_array_t* out
 */
#define SLL_INIT_ARRAY(out) \
	do{ \
		(out)->length=0; \
		(out)->data=NULL; \
	} while (0)



/**
 * \flags macro var
 * \name SLL_INIT_ARRAY_STRUCT
 * \group array
 * \desc Docs!
 * \type sll_array_t
 */
#define SLL_INIT_ARRAY_STRUCT {0,NULL}



/**
 * \flags func
 * \name sll_array_and
 * \group array
 * \subgroup array-op
 * \desc Docs!
 * \arg const sll_array_t* a
 * \arg const sll_array_t* b
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL void sll_array_and(const sll_array_t* a,const sll_array_t* b,sll_array_t* out);



/**
 * \flags func
 * \name sll_array_clone
 * \group array
 * \desc Docs!
 * \arg const sll_array_t* array
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL void sll_array_clone(const sll_array_t* array,sll_array_t* out);



/**
 * \flags check_output func
 * \name sll_array_count
 * \group array
 * \subgroup array-op
 * \desc Docs!
 * \arg const sll_array_t* array
 * \arg sll_object_t* object
 * \ret sll_array_length_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_array_length_t sll_array_count(const sll_array_t* array,sll_object_t* object);



/**
 * \flags check_output func
 * \name sll_array_count_multiple
 * \group array
 * \subgroup array-op
 * \desc Docs!
 * \arg const sll_array_t* array
 * \arg sll_object_t*const* object_data
 * \arg sll_array_length_t object_count
 * \ret sll_array_length_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_array_length_t sll_array_count_multiple(const sll_array_t* array,sll_object_t*const* object_data,sll_array_length_t object_count);



/**
 * \flags func
 * \name sll_array_combinations
 * \group array
 * \subgroup array-data
 * \desc Docs!
 * \arg const sll_array_t* a
 * \arg const sll_array_t* b
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL void sll_array_combinations(const sll_array_t* a,const sll_array_t* b,sll_array_t* out);



/**
 * \flags func
 * \name sll_array_combinations_string
 * \group array
 * \subgroup array-data
 * \desc Docs!
 * \arg const sll_array_t* array
 * \arg const sll_string_t* string
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL void sll_array_combinations_string(const sll_array_t* array,const sll_string_t* string,sll_array_t* out);



/**
 * \flags check_output func
 * \name sll_array_compare
 * \group array
 * \desc Docs!
 * \arg const sll_array_t* a
 * \arg const sll_array_t* b
 * \ret sll_compare_result_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_compare_result_t sll_array_compare(const sll_array_t* a,const sll_array_t* b);



/**
 * \flags check_output func
 * \name sll_array_compare_map
 * \group array
 * \desc Docs!
 * \arg const sll_array_t* array
 * \arg const sll_map_t* map
 * \arg sll_bool_t inv
 * \ret sll_compare_result_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_compare_result_t sll_array_compare_map(const sll_array_t* array,const sll_map_t* map,sll_bool_t inv);



/**
 * \flags func
 * \name sll_array_create
 * \group array
 * \desc Docs!
 * \arg sll_array_length_t length
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL void sll_array_create(sll_array_length_t length,sll_array_t* out);



/**
 * \flags func
 * \name sll_array_create_zero
 * \group array
 * \desc Docs!
 * \arg sll_array_length_t length
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL void sll_array_create_zero(sll_array_length_t length,sll_array_t* out);



/**
 * \flags func
 * \name sll_array_duplicate
 * \group array
 * \desc Docs!
 * \arg const sll_array_t* array
 * \arg sll_integer_t count
 * \arg sll_array_length_t extra
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL void sll_array_duplicate(const sll_array_t* array,sll_integer_t count,sll_array_length_t extra,sll_array_t* out);



/**
 * \flags check_output func
 * \name sll_array_equal
 * \group array
 * \subgroup array-op
 * \desc Docs!
 * \arg const sll_array_t* a
 * \arg const sll_array_t* b
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_array_equal(const sll_array_t* a,const sll_array_t* b);



/**
 * \flags check_output func
 * \name sll_array_equal_map
 * \group array
 * \subgroup array-op
 * \desc Docs!
 * \arg const sll_array_t* array
 * \arg const sll_map_t* map
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_array_equal_map(const sll_array_t* array,const sll_map_t* map);



/**
 * \flags func
 * \name sll_array_extend
 * \group array
 * \subgroup array-op
 * \desc Docs!
 * \arg const sll_array_t* base
 * \arg const sll_array_t* new
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL void sll_array_extend(const sll_array_t* base,const sll_array_t* new,sll_array_t* out);



/**
 * \flags check_output func
 * \name sll_array_get
 * \group array
 * \subgroup array-data
 * \desc Docs!
 * \arg const sll_array_t* array
 * \arg sll_array_length_t index
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_array_get(const sll_array_t* array,sll_array_length_t index);



/**
 * \flags check_output func
 * \name sll_array_includes
 * \group array
 * \desc Docs!
 * \arg const sll_array_t* array
 * \arg sll_object_t* object
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_array_includes(const sll_array_t* array,sll_object_t* object);



/**
 * \flags func
 * \name sll_array_join_arrays
 * \group array
 * \subgroup array-op
 * \desc Docs!
 * \arg const sll_array_t*const* array_data
 * \arg sll_array_length_t array_count
 * \arg sll_object_t* infix
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL void sll_array_join_arrays(const sll_array_t*const* array_data,sll_array_length_t array_count,sll_object_t* infix,sll_array_t* out);



/**
 * \flags func
 * \name sll_array_op
 * \group array
 * \subgroup array-op
 * \desc Docs!
 * \arg const sll_array_t* a
 * \arg const sll_array_t* b
 * \arg sll_binary_operator_t operator
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL void sll_array_op(const sll_array_t* a,const sll_array_t* b,sll_binary_operator_t operator,sll_array_t* out);



/**
 * \flags func
 * \name sll_array_op_map
 * \group array
 * \subgroup array-op
 * \desc Docs!
 * \arg const sll_array_t* array
 * \arg const sll_map_t* map
 * \arg sll_binary_operator_t operator
 * \arg sll_bool_t inv
 * \arg sll_map_t* out
 */
__SLL_EXTERNAL void sll_array_op_map(const sll_array_t* array,const sll_map_t* map,sll_binary_operator_t operator,sll_bool_t inv,sll_map_t* out);



/**
 * \flags func
 * \name sll_array_or
 * \group array
 * \subgroup array-op
 * \desc Docs!
 * \arg const sll_array_t* a
 * \arg const sll_array_t* b
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL void sll_array_or(const sll_array_t* a,const sll_array_t* b,sll_array_t* out);



/**
 * \flags check_output func
 * \name sll_array_parse_char
 * \group array
 * \desc Docs!
 * \arg const sll_array_t* array
 * \ret sll_char_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_char_t sll_array_parse_char(const sll_array_t* array);



/**
 * \flags check_output func
 * \name sll_array_parse_float
 * \group array
 * \desc Docs!
 * \arg const sll_array_t* array
 * \ret sll_float_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_float_t sll_array_parse_float(const sll_array_t* array);



/**
 * \flags check_output func
 * \name sll_array_parse_int
 * \group array
 * \desc Docs!
 * \arg const sll_array_t* array
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_integer_t sll_array_parse_int(const sll_array_t* array);



/**
 * \flags check_output func
 * \name sll_array_pop
 * \group array
 * \subgroup array-data
 * \desc Docs!
 * \arg const sll_array_t* array
 * \arg sll_array_t* out
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_array_pop(const sll_array_t* array,sll_array_t* out);



/**
 * \flags func
 * \name sll_array_push
 * \group array
 * \subgroup array-data
 * \desc Docs!
 * \arg const sll_array_t* array
 * \arg sll_object_t* object
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL void sll_array_push(const sll_array_t* array,sll_object_t* object,sll_array_t* out);



/**
 * \flags func
 * \name sll_array_range
 * \group array
 * \desc Docs!
 * \arg sll_integer_t start
 * \arg sll_integer_t end
 * \arg sll_integer_t step
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL void sll_array_range(sll_integer_t start,sll_integer_t end,sll_integer_t step,sll_array_t* out);



/**
 * \flags func
 * \name sll_array_remove
 * \group array
 * \subgroup array-data
 * \desc Docs!
 * \arg const sll_array_t* array
 * \arg sll_object_t* object
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL void sll_array_remove(const sll_array_t* array,sll_object_t* object,sll_array_t* out);



/**
 * \flags func
 * \name sll_array_remove_multiple
 * \group array
 * \subgroup array-data
 * \desc Docs!
 * \arg const sll_array_t* array
 * \arg sll_object_t** object_data
 * \arg sll_array_length_t object_count
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL void sll_array_remove_multiple(const sll_array_t* array,sll_object_t** object_data,sll_array_length_t object_count,sll_array_t* out);



/**
 * \flags func
 * \name sll_array_replace
 * \group array
 * \subgroup array-data
 * \desc Docs!
 * \arg const sll_array_t* array
 * \arg sll_object_t* key
 * \arg sll_object_t* value
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL void sll_array_replace(const sll_array_t* array,sll_object_t* key,sll_object_t* value,sll_array_t* out);



/**
 * \flags func
 * \name sll_array_resize
 * \group array
 * \subgroup array-data
 * \desc Docs!
 * \arg const sll_array_t* array
 * \arg sll_integer_t delta
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL void sll_array_resize(const sll_array_t* array,sll_integer_t delta,sll_array_t* out);



/**
 * \flags func
 * \name sll_array_reverse
 * \group array
 * \subgroup array-op
 * \desc Docs!
 * \arg const sll_array_t* array
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL void sll_array_reverse(const sll_array_t* array,sll_array_t* out);



/**
 * \flags func
 * \name sll_array_select
 * \group array
 * \subgroup array-op
 * \desc Docs!
 * \arg const sll_array_t* array
 * \arg sll_integer_t start
 * \arg sll_integer_t end
 * \arg sll_integer_t step
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL void sll_array_select(const sll_array_t* array,sll_integer_t start,sll_integer_t end,sll_integer_t step,sll_array_t* out);



/**
 * \flags func
 * \name sll_array_split
 * \group array
 * \subgroup array-op
 * \desc Docs!
 * \arg const sll_array_t* array
 * \arg sll_object_t* object
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL void sll_array_split(const sll_array_t* array,sll_object_t* object,sll_array_t* out);



/**
 * \flags func
 * \name sll_array_set
 * \group array
 * \subgroup array-data
 * \desc Docs!
 * \arg const sll_array_t* array
 * \arg sll_array_length_t index
 * \arg sll_object_t* object
 */
__SLL_EXTERNAL void sll_array_set(const sll_array_t* array,sll_array_length_t index,sll_object_t* object);



/**
 * \flags check_output func
 * \name sll_array_shift
 * \group array
 * \subgroup array-data
 * \desc Docs!
 * \arg const sll_array_t* a
 * \arg sll_array_t* o
 * \arg sll_object_t* out
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_array_shift(const sll_array_t* a,sll_array_t* o);



/**
 * \flags func
 * \name sll_array_to_map
 * \group array
 * \desc Docs!
 * \arg const sll_array_t* a
 * \arg sll_map_t* o
 */
__SLL_EXTERNAL void sll_array_to_map(const sll_array_t* a,sll_map_t* o);



/**
 * \flags func
 * \name sll_array_unshift
 * \group array
 * \subgroup array-data
 * \desc Docs!
 * \arg const sll_array_t* a
 * \arg sll_object_t* v
 * \arg sll_array_t* o
 */
__SLL_EXTERNAL void sll_array_unshift(const sll_array_t* a,sll_object_t* v,sll_array_t* o);



/**
 * \flags func
 * \name sll_array_xor
 * \group array
 * \subgroup array-op
 * \desc Docs!
 * \arg const sll_array_t* a
 * \arg const sll_array_t* b
 * \arg sll_array_t* o
 */
__SLL_EXTERNAL void sll_array_xor(const sll_array_t* a,const sll_array_t* b,sll_array_t* o);



/**
 * \flags func
 * \name sll_free_array
 * \group array
 * \desc Docs!
 * \arg sll_array_t* a
 */
__SLL_EXTERNAL void sll_free_array(sll_array_t* a);



#endif

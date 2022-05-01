#ifndef __SLL_MAP_H__
#define __SLL_MAP_H__ 1
#include <sll/_size_types.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/operator.h>
#include <sll/string.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Map
 * \group map
 * \desc Docs!
 */
/**
 * \flags subgroup
 * \name Content
 * \group map
 * \subgroup map-data
 * \desc Docs!
 */
/**
 * \flags subgroup
 * \name Operations
 * \group map
 * \subgroup map-op
 * \desc Docs!
 */



/**
 * \flags macro var
 * \name SLL_MAX_MAP_LENGTH
 * \group map
 * \desc Docs!
 * \type sll_map_length_t
 */
#define SLL_MAX_MAP_LENGTH __SLL_U32_MAX



/**
 * \flags func macro
 * \name SLL_INIT_MAP
 * \group map
 * \desc Docs!
 * \arg sll_map_t* o
 */
#define SLL_INIT_MAP(o) \
	do{ \
		(o)->l=0; \
		(o)->v=NULL; \
	} while (0)



/**
 * \flags macro var
 * \name SLL_INIT_MAP_STRUCT
 * \group map
 * \desc Docs!
 * \type sll_map_t
 */
#define SLL_INIT_MAP_STRUCT {0,NULL}



/**
 * \flags func
 * \name sll_free_map
 * \group map
 * \desc Docs!
 * \arg sll_map_t* m
 */
__SLL_EXTERNAL void sll_free_map(sll_map_t* m);



/**
 * \flags func
 * \name sll_map_add
 * \group map
 * \subgroup map-data
 * \desc Docs!
 * \arg const sll_map_t* m
 * \arg sll_object_t* k
 * \arg sll_object_t* v
 * \arg sll_map_t* o
 */
__SLL_EXTERNAL void sll_map_add(const sll_map_t* m,sll_object_t* k,sll_object_t* v,sll_map_t* o);



/**
 * \flags func
 * \name sll_map_add_array
 * \group map
 * \subgroup map-data
 * \desc Docs!
 * \arg const sll_map_t* m
 * \arg const sll_array_t* a
 * \arg sll_map_t* o
 */
__SLL_EXTERNAL void sll_map_add_array(const sll_map_t* m,const sll_array_t* a,sll_map_t* o);



/**
 * \flags func
 * \name sll_map_add_string
 * \group map
 * \subgroup map-data
 * \desc Docs!
 * \arg const sll_map_t* m
 * \arg const sll_string_t* s
 * \arg sll_map_t* o
 */
__SLL_EXTERNAL void sll_map_add_string(const sll_map_t* m,const sll_string_t* s,sll_map_t* o);



/**
 * \flags func
 * \name sll_map_and
 * \group map
 * \subgroup map-op
 * \desc Docs!
 * \arg const sll_map_t* a
 * \arg const sll_map_t* b
 * \arg sll_map_t* o
 */
__SLL_EXTERNAL void sll_map_and(const sll_map_t* a,const sll_map_t* b,sll_map_t* o);



/**
 * \flags func
 * \name sll_map_and_array
 * \group map
 * \subgroup map-op
 * \desc Docs!
 * \arg const sll_map_t* m
 * \arg const sll_array_t* a
 * \arg sll_map_t* o
 */
__SLL_EXTERNAL void sll_map_and_array(const sll_map_t* m,const sll_array_t* a,sll_map_t* o);



/**
 * \flags func
 * \name sll_map_and_string
 * \group map
 * \subgroup map-op
 * \desc Docs!
 * \arg const sll_map_t* m
 * \arg const sll_string_t* s
 * \arg sll_map_t* o
 */
__SLL_EXTERNAL void sll_map_and_string(const sll_map_t* m,const sll_string_t* s,sll_map_t* o);



/**
 * \flags func
 * \name sll_map_clone
 * \group map
 * \desc Docs!
 * \arg const sll_map_t* m
 * \arg sll_map_t* o
 */
__SLL_EXTERNAL void sll_map_clone(const sll_map_t* m,sll_map_t* o);



/**
 * \flags check_output func
 * \name sll_map_compare
 * \group map
 * \subgroup map-op
 * \desc Docs!
 * \arg const sll_map_t* a
 * \arg const sll_map_t* b
 * \ret sll_compare_result_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_compare_result_t sll_map_compare(const sll_map_t* a,const sll_map_t* b);



/**
 * \flags func
 * \name sll_map_create
 * \group map
 * \desc Docs!
 * \arg sll_map_length_t l
 * \arg sll_map_t* o
 */
__SLL_EXTERNAL void sll_map_create(sll_map_length_t l,sll_map_t* o);



/**
 * \flags check_output func
 * \name sll_map_equal
 * \group map
 * \subgroup map-op
 * \desc Docs!
 * \arg const sll_map_t* a
 * \arg const sll_map_t* b
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_map_equal(const sll_map_t* a,const sll_map_t* b);



/**
 * \flags check_output func
 * \name sll_map_get
 * \group map
 * \subgroup map-data
 * \desc Docs!
 * \arg const sll_map_t* m
 * \arg sll_object_t* k
 * \arg sll_object_t* out
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_map_get(const sll_map_t* m,sll_object_t* k);



/**
 * \flags check_output func
 * \name sll_map_get_key
 * \group map
 * \subgroup map-data
 * \desc Docs!
 * \arg const sll_map_t* m
 * \arg sll_map_length_t i
 * \arg sll_object_t* out
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_map_get_key(const sll_map_t* m,sll_map_length_t i);



/**
 * \flags check_output func
 * \name sll_map_get_value
 * \group map
 * \subgroup map-data
 * \desc Docs!
 * \arg const sll_map_t* m
 * \arg sll_map_length_t i
 * \arg sll_object_t* out
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_map_get_value(const sll_map_t* m,sll_map_length_t i);



/**
 * \flags check_output func
 * \name sll_map_includes
 * \group map
 * \desc Docs!
 * \arg const sll_map_t* m
 * \arg sll_object_t* e
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_map_includes(const sll_map_t* m,sll_object_t* e);



/**
 * \flags func
 * \name sll_map_join
 * \group map
 * \subgroup map-op
 * \desc Docs!
 * \arg const sll_map_t* a
 * \arg const sll_map_t* b
 * \arg sll_map_t* o
 */
__SLL_EXTERNAL void sll_map_join(const sll_map_t* a,const sll_map_t* b,sll_map_t* o);



/**
 * \flags func
 * \name sll_map_keys
 * \group map
 * \desc Docs!
 * \arg const sll_map_t* m
 * \arg sll_array_t* o
 */
__SLL_EXTERNAL void sll_map_keys(const sll_map_t* m,sll_array_t* o);



/**
 * \flags func
 * \name sll_map_op
 * \group map
 * \subgroup map-op
 * \desc Docs!
 * \arg const sll_map_t* a
 * \arg const sll_map_t* b
 * \arg sll_binary_operator_t f
 * \arg sll_map_t* o
 */
__SLL_EXTERNAL void sll_map_op(const sll_map_t* a,const sll_map_t* b,sll_binary_operator_t f,sll_map_t* o);



/**
 * \flags check_output func
 * \name sll_map_remove
 * \group map
 * \subgroup map-data
 * \desc Docs!
 * \arg const sll_map_t* m
 * \arg sll_object_t* k
 * \arg sll_map_t* o
 * \arg sll_object_t* out
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_map_remove(const sll_map_t* m,sll_object_t* k,sll_map_t* o);



/**
 * \flags func
 * \name sll_map_remove_array
 * \group map
 * \subgroup map-data
 * \desc Docs!
 * \arg const sll_map_t* m
 * \arg const sll_array_t* a
 * \arg sll_map_t* o
 */
__SLL_EXTERNAL void sll_map_remove_array(const sll_map_t* m,const sll_array_t* a,sll_map_t* o);



/**
 * \flags func
 * \name sll_map_remove_map
 * \group map
 * \subgroup map-data
 * \desc Docs!
 * \arg const sll_map_t* a
 * \arg const sll_map_t* b
 * \arg sll_map_t* o
 */
__SLL_EXTERNAL void sll_map_remove_map(const sll_map_t* a,const sll_map_t* b,sll_map_t* o);



/**
 * \flags func
 * \name sll_map_remove_string
 * \group map
 * \subgroup map-data
 * \desc Docs!
 * \arg const sll_map_t* m
 * \arg const sll_string_t* s
 * \arg sll_map_t* o
 */
__SLL_EXTERNAL void sll_map_remove_string(const sll_map_t* m,const sll_string_t* s,sll_map_t* o);



/**
 * \flags func
 * \name sll_map_set
 * \group map
 * \subgroup map-data
 * \desc Docs!
 * \arg sll_map_t* m
 * \arg sll_object_t* k
 * \arg sll_object_t* v
 */
__SLL_EXTERNAL void sll_map_set(sll_map_t* m,sll_object_t* k,sll_object_t* v);



/**
 * \flags func
 * \name sll_map_set_key
 * \group map
 * \subgroup map-data
 * \desc Docs!
 * \arg const sll_map_t* m
 * \arg sll_map_length_t i
 * \arg sll_object_t* v
 */
__SLL_EXTERNAL void sll_map_set_key(const sll_map_t* m,sll_map_length_t i,sll_object_t* v);



/**
 * \flags func
 * \name sll_map_set_value
 * \group map
 * \subgroup map-data
 * \desc Docs!
 * \arg const sll_map_t* m
 * \arg sll_map_length_t i
 * \arg sll_object_t* v
 */
__SLL_EXTERNAL void sll_map_set_value(const sll_map_t* m,sll_map_length_t i,sll_object_t* v);



/**
 * \flags func
 * \name sll_map_to_array
 * \group map
 * \desc Docs!
 * \arg const sll_map_t* m
 * \arg sll_array_t* o
 */
__SLL_EXTERNAL void sll_map_to_array(const sll_map_t* m,sll_array_t* o);



/**
 * \flags func
 * \name sll_map_values
 * \group map
 * \desc Docs!
 * \arg const sll_map_t* m
 * \arg sll_array_t* o
 */
__SLL_EXTERNAL void sll_map_values(const sll_map_t* m,sll_array_t* o);



#endif

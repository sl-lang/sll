#ifndef __SLL_MAP_H__
#define __SLL_MAP_H__ 1
#include <sll/common.h>
#include <sll/types.h>



#define SLL_INIT_MAP(o) \
	do{ \
		(o)->l=0; \
		(o)->v=NULL; \
	} while (0)
#define SLL_INIT_MAP_STRUCT {0,NULL}



/**
 * \flags func
 * \name sll_free_map
 * \desc Docs!
 * \arg sll_map_t* m
 */
__SLL_EXTERNAL void sll_free_map(sll_map_t* m);



/**
 * \flags func
 * \name sll_map_add
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
 * \desc Docs!
 * \arg const sll_map_t* m
 * \arg const sll_array_t* a
 * \arg sll_map_t* o
 */
__SLL_EXTERNAL void sll_map_add_array(const sll_map_t* m,const sll_array_t* a,sll_map_t* o);



/**
 * \flags func
 * \name sll_map_add_string
 * \desc Docs!
 * \arg const sll_map_t* m
 * \arg const sll_string_t* s
 * \arg sll_map_t* o
 */
__SLL_EXTERNAL void sll_map_add_string(const sll_map_t* m,const sll_string_t* s,sll_map_t* o);



/**
 * \flags func
 * \name sll_map_and
 * \desc Docs!
 * \arg const sll_map_t* a
 * \arg const sll_map_t* b
 * \arg sll_map_t* o
 */
__SLL_EXTERNAL void sll_map_and(const sll_map_t* a,const sll_map_t* b,sll_map_t* o);



/**
 * \flags func
 * \name sll_map_and_array
 * \desc Docs!
 * \arg const sll_map_t* m
 * \arg const sll_array_t* a
 * \arg sll_map_t* o
 */
__SLL_EXTERNAL void sll_map_and_array(const sll_map_t* m,const sll_array_t* a,sll_map_t* o);



/**
 * \flags func
 * \name sll_map_and_string
 * \desc Docs!
 * \arg const sll_map_t* m
 * \arg const sll_string_t* s
 * \arg sll_map_t* o
 */
__SLL_EXTERNAL void sll_map_and_string(const sll_map_t* m,const sll_string_t* s,sll_map_t* o);



/**
 * \flags func
 * \name sll_map_clone
 * \desc Docs!
 * \arg const sll_map_t* m
 * \arg sll_map_t* o
 */
__SLL_EXTERNAL void sll_map_clone(const sll_map_t* m,sll_map_t* o);



/**
 * \flags check_output func
 * \name sll_map_compare
 * \desc Docs!
 * \arg const sll_map_t* a
 * \arg const sll_map_t* b
 * \ret sll_compare_result_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_compare_result_t sll_map_compare(const sll_map_t* a,const sll_map_t* b);



/**
 * \flags func
 * \name sll_map_create
 * \desc Docs!
 * \arg sll_map_length_t l
 * \arg sll_map_t* o
 */
__SLL_EXTERNAL void sll_map_create(sll_map_length_t l,sll_map_t* o);



/**
 * \flags check_output func
 * \name sll_map_equal
 * \desc Docs!
 * \arg const sll_map_t* a
 * \arg const sll_map_t* b
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_map_equal(const sll_map_t* a,const sll_map_t* b);



/**
 * \flags check_output func
 * \name sll_map_get
 * \desc Docs!
 * \arg const sll_map_t* m
 * \arg const sll_object_t* v
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_map_get(const sll_map_t* m,const sll_object_t* v);



/**
 * \flags check_output func
 * \name sll_map_get_key
 * \desc Docs!
 * \arg const sll_map_t* m
 * \arg sll_map_length_t i
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_map_get_key(const sll_map_t* m,sll_map_length_t i);



/**
 * \flags check_output func
 * \name sll_map_get_value
 * \desc Docs!
 * \arg const sll_map_t* m
 * \arg sll_map_length_t i
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_map_get_value(const sll_map_t* m,sll_map_length_t i);



/**
 * \flags func
 * \name sll_map_join
 * \desc Docs!
 * \arg const sll_map_t* a
 * \arg const sll_map_t* b
 * \arg sll_map_t* o
 */
__SLL_EXTERNAL void sll_map_join(const sll_map_t* a,const sll_map_t* b,sll_map_t* o);



/**
 * \flags func
 * \name sll_map_keys
 * \desc Docs!
 * \arg const sll_map_t* m
 * \arg sll_array_t* o
 */

__SLL_EXTERNAL void sll_map_keys(const sll_map_t* m,sll_array_t* o);



/**
 * \flags func
 * \name sll_map_op
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
 * \desc Docs!
 * \arg const sll_map_t* m
 * \arg sll_object_t* k
 * \arg sll_map_t* o
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_map_remove(const sll_map_t* m,sll_object_t* k,sll_map_t* o);



/**
 * \flags func
 * \name sll_map_remove_array
 * \desc Docs!
 * \arg const sll_map_t* m
 * \arg const sll_array_t* a
 * \arg sll_map_t* o
 */
__SLL_EXTERNAL void sll_map_remove_array(const sll_map_t* m,const sll_array_t* a,sll_map_t* o);



/**
 * \flags func
 * \name sll_map_remove_map
 * \desc Docs!
 * \arg const sll_map_t* a
 * \arg const sll_map_t* b
 * \arg sll_map_t* o
 */
__SLL_EXTERNAL void sll_map_remove_map(const sll_map_t* a,const sll_map_t* b,sll_map_t* o);



/**
 * \flags func
 * \name sll_map_remove_string
 * \desc Docs!
 * \arg const sll_map_t* m
 * \arg const sll_string_t* s
 * \arg sll_map_t* o
 */
__SLL_EXTERNAL void sll_map_remove_string(const sll_map_t* m,const sll_string_t* s,sll_map_t* o);



/**
 * \flags func
 * \name sll_map_set
 * \desc Docs!
 * \arg const sll_map_t* m
 * \arg sll_object_t* k
 * \arg sll_object_t* v
 */
__SLL_EXTERNAL void sll_map_set(const sll_map_t* m,sll_object_t* k,sll_object_t* v);



/**
 * \flags func
 * \name sll_map_set_key
 * \desc Docs!
 * \arg const sll_map_t* m
 * \arg sll_map_length_t i
 * \arg sll_object_t* v
 */

__SLL_EXTERNAL void sll_map_set_key(const sll_map_t* m,sll_map_length_t i,sll_object_t* v);



/**
 * \flags func
 * \name sll_map_set_value
 * \desc Docs!
 * \arg const sll_map_t* m
 * \arg sll_map_length_t i
 * \arg sll_object_t* v
 */
__SLL_EXTERNAL void sll_map_set_value(const sll_map_t* m,sll_map_length_t i,sll_object_t* v);



/**
 * \flags func
 * \name sll_map_to_array
 * \desc Docs!
 * \arg const sll_map_t* m
 * \arg sll_array_t* o
 */
__SLL_EXTERNAL void sll_map_to_array(const sll_map_t* m,sll_array_t* o);



/**
 * \flags func
 * \name sll_map_values
 * \desc Docs!
 * \arg const sll_map_t* m
 * \arg sll_array_t* o
 */
__SLL_EXTERNAL void sll_map_values(const sll_map_t* m,sll_array_t* o);



#endif

#ifndef __SLL_MAP_H__
#define __SLL_MAP_H__ 1
#include <sll/common.h>
#include <sll/types.h>



__SLL_FUNC void sll_map_add(const sll_map_t* m,sll_runtime_object_t* k,sll_runtime_object_t* v,sll_map_t* o);



__SLL_FUNC void sll_map_add_array(const sll_map_t* m,const sll_array_t* a,sll_map_t* o);



__SLL_FUNC void sll_map_add_string(const sll_map_t* m,const sll_string_t* s,sll_map_t* o);



__SLL_FUNC void sll_map_and(const sll_map_t* a,const sll_map_t* b,sll_map_t* o);



__SLL_FUNC void sll_map_and_array(const sll_map_t* m,const sll_array_t* a,sll_map_t* o);



__SLL_FUNC void sll_map_and_string(const sll_map_t* m,const sll_string_t* s,sll_map_t* o);



__SLL_FUNC void sll_map_clone(const sll_map_t* m,sll_map_t* o);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_compare_result_t sll_map_compare(const sll_map_t* a,const sll_map_t* b);



__SLL_FUNC void sll_map_create(sll_map_length_t l,sll_map_t* o);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_bool_t sll_map_equal(const sll_map_t* a,const sll_map_t* b);



__SLL_FUNC void sll_map_join(const sll_map_t* a,const sll_map_t* b,sll_map_t* o);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_map_get(const sll_map_t* m,const sll_runtime_object_t* v);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_map_get_key(const sll_map_t* m,sll_map_length_t i);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_map_get_value(const sll_map_t* m,sll_map_length_t i);



__SLL_FUNC void sll_map_keys(const sll_map_t* m,sll_array_t* o);



__SLL_FUNC void sll_map_op(const sll_map_t* a,const sll_map_t* b,sll_binary_operator_t f,sll_map_t* o);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_map_remove(const sll_map_t* m,sll_runtime_object_t* k,sll_map_t* o);



__SLL_FUNC void sll_map_remove_array(const sll_map_t* m,const sll_array_t* a,sll_map_t* o);



__SLL_FUNC void sll_map_remove_map(const sll_map_t* a,const sll_map_t* b,sll_map_t* o);



__SLL_FUNC void sll_map_remove_string(const sll_map_t* m,const sll_string_t* s,sll_map_t* o);



__SLL_FUNC void sll_map_set(const sll_map_t* m,sll_runtime_object_t* k,sll_runtime_object_t* v);



__SLL_FUNC void sll_map_set_key(const sll_map_t* m,sll_map_length_t i,sll_runtime_object_t* v);



__SLL_FUNC void sll_map_set_value(const sll_map_t* m,sll_map_length_t i,sll_runtime_object_t* v);



__SLL_FUNC void sll_map_to_array(const sll_map_t* m,sll_array_t* o);



__SLL_FUNC void sll_map_values(const sll_map_t* m,sll_array_t* o);



#endif

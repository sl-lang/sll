#ifndef __SLL_ARRAY_H__
#define __SLL_ARRAY_H__ 1
#include <sll/common.h>
#include <sll/types.h>



#define SLL_ZERO_ARRAY(a) \
	do{ \
		(a)->l=0; \
		(a)->v=NULL; \
	} while (0)
#define SLL_ZERO_ARRAY_STRUCT {0,NULL}



__SLL_FUNC void sll_array_and(const sll_array_t* a,const sll_array_t* b,sll_array_t* o);



__SLL_FUNC void sll_array_clone(const sll_array_t* a,sll_array_t* o);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_array_length_t sll_array_count(const sll_array_t* a,sll_runtime_object_t* v);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_array_length_t sll_array_count_multiple(const sll_array_t* a,sll_runtime_object_t** v,sll_array_length_t vl);



__SLL_FUNC void sll_array_combinations(const sll_array_t* a,const sll_array_t* b,sll_array_t* o);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_compare_result_t sll_array_compare(const sll_array_t* a,const sll_array_t* b);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_compare_result_t sll_array_compare_map(const sll_array_t* a,const sll_map_t* m,sll_bool_t inv);



__SLL_FUNC void sll_array_create(sll_array_length_t l,sll_array_t* o);



__SLL_FUNC void sll_array_duplicate(const sll_array_t* a,sll_integer_t n,sll_array_length_t e,sll_array_t* o);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_bool_t sll_array_equal(const sll_array_t* a,const sll_array_t* b);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_bool_t sll_array_equal_map(const sll_array_t* a,const sll_map_t* m);



__SLL_FUNC void sll_array_from_length(sll_array_length_t l,sll_array_t* o);



__SLL_FUNC void sll_array_join(const sll_array_t* a,const sll_array_t* b,sll_array_t* o);



__SLL_FUNC void sll_array_op(const sll_array_t* a,const sll_array_t* b,sll_binary_operator_t f,sll_array_t* o);



__SLL_FUNC void sll_array_op_map(const sll_array_t* a,const sll_map_t* m,sll_binary_operator_t f,sll_map_t* o);



__SLL_FUNC void sll_array_or(const sll_array_t* a,const sll_array_t* b,sll_array_t* o);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_array_pop(const sll_array_t* a,sll_array_t* o);



__SLL_FUNC void sll_array_push(const sll_array_t* a,sll_runtime_object_t* v,sll_array_t* o);



__SLL_FUNC void sll_array_range(sll_integer_t s,sll_integer_t e,sll_integer_t n,sll_array_t* o);



__SLL_FUNC void sll_array_remove(const sll_array_t* a,sll_runtime_object_t* v,sll_array_t* o);



__SLL_FUNC void sll_array_remove_multiple(const sll_array_t* a,sll_runtime_object_t** v,sll_array_length_t vl,sll_array_t* o);



__SLL_FUNC void sll_array_resize(const sll_array_t* a,sll_integer_t v,sll_array_t* o);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_array_shift(const sll_array_t* a,sll_array_t* o);



__SLL_FUNC void sll_array_string_combinations(const sll_array_t* a,const sll_string_t* s,sll_array_t* o);



__SLL_FUNC void sll_array_to_map(const sll_array_t* a,sll_map_t* o);



__SLL_FUNC void sll_array_unshift(const sll_array_t* a,sll_runtime_object_t* v,sll_array_t* o);



__SLL_FUNC void sll_array_xor(const sll_array_t* a,const sll_array_t* b,sll_array_t* o);



#endif

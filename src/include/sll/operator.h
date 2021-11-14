#ifndef __SLL_OPERATOR_H__
#define __SLL_OPERATOR_H__ 1
#include <sll/common.h>
#include <sll/types.h>
#include <math.h>



#define SLL_FLOAT_ABSOLUTE(v) fabs((v))
#define SLL_FLOAT_ROUND(v) llround((v))



__SLL_EXTERNAL extern const sll_float_t sll_float_compare_error;



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_operator_access(sll_runtime_object_t* a,sll_runtime_object_t* b);



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_operator_access_range(sll_runtime_object_t* a,sll_runtime_object_t* b,sll_runtime_object_t* c);



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_operator_access_range_step(sll_runtime_object_t* a,sll_runtime_object_t* b,sll_runtime_object_t* c,sll_runtime_object_t* d);



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_operator_add(sll_runtime_object_t* a,sll_runtime_object_t* b);



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_operator_and(sll_runtime_object_t* a,sll_runtime_object_t* b);



__SLL_EXTERNAL void sll_operator_assign(sll_runtime_object_t* a,sll_runtime_object_t* b,sll_runtime_object_t* v);



__SLL_EXTERNAL void sll_operator_assign_range(sll_runtime_object_t* a,sll_runtime_object_t* b,sll_runtime_object_t* c,sll_runtime_object_t* v);



__SLL_EXTERNAL void sll_operator_assign_range_step(sll_runtime_object_t* a,sll_runtime_object_t* b,sll_runtime_object_t* c,sll_runtime_object_t* d,sll_runtime_object_t* v);



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_operator_bool(const sll_runtime_object_t* a);



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_operator_cast(sll_runtime_object_t* a,sll_runtime_object_t* b);



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_compare_result_t sll_operator_compare(const sll_runtime_object_t* a,const sll_runtime_object_t* b);



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_operator_dec(sll_runtime_object_t* a);



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_operator_div(sll_runtime_object_t* a,sll_runtime_object_t* b);



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_operator_dup(sll_runtime_object_t* a);



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_operator_equal(const sll_runtime_object_t* a,const sll_runtime_object_t* b);



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_operator_floor_div(sll_runtime_object_t* a,sll_runtime_object_t* b);



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_operator_inc(sll_runtime_object_t* a);



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_operator_inv(sll_runtime_object_t* a);



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_operator_len(sll_runtime_object_t* a);



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_operator_mod(sll_runtime_object_t* a,sll_runtime_object_t* b);



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_operator_mult(sll_runtime_object_t* a,sll_runtime_object_t* b);



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_operator_or(sll_runtime_object_t* a,sll_runtime_object_t* b);



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_operator_shl(sll_runtime_object_t* a,sll_runtime_object_t* b);



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_operator_shr(sll_runtime_object_t* a,sll_runtime_object_t* b);



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_operator_strict_equal(const sll_runtime_object_t* a,const sll_runtime_object_t* b);



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_operator_sub(sll_runtime_object_t* a,sll_runtime_object_t* b);



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_operator_xor(sll_runtime_object_t* a,sll_runtime_object_t* b);



#endif

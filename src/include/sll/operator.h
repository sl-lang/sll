#ifndef __SLL_OPERATOR_H__
#define __SLL_OPERATOR_H__ 1
#include <sll/common.h>
#include <sll/types.h>
#include <math.h>



#define __SLL_OPERATOR_UNARY(nm) __SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_operator_##nm(sll_runtime_object_t* a)
#define __SLL_OPERATOR_BINARY(nm) __SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_operator_##nm(sll_runtime_object_t* a,sll_runtime_object_t* b)
#define __SLL_OPERATOR_TERNARY(nm) __SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_operator_##nm(sll_runtime_object_t* a,sll_runtime_object_t* b,sll_runtime_object_t* c)
#define __SLL_OPERATOR_QUATERNARY(nm) __SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_operator_##nm(sll_runtime_object_t* a,sll_runtime_object_t* b,sll_runtime_object_t* c,sll_runtime_object_t* d)



#define SLL_ROUND_FLOAT(v) llround((v))



extern const sll_float_t sll_float_compare_error;



__SLL_OPERATOR_UNARY(inc);



__SLL_OPERATOR_UNARY(dec);



__SLL_OPERATOR_BINARY(add);



__SLL_OPERATOR_BINARY(sub);



__SLL_OPERATOR_BINARY(mult);



__SLL_OPERATOR_BINARY(div);



__SLL_OPERATOR_BINARY(floor_div);



__SLL_OPERATOR_BINARY(mod);



__SLL_OPERATOR_BINARY(and);



__SLL_OPERATOR_BINARY(or);



__SLL_OPERATOR_BINARY(xor);



__SLL_OPERATOR_UNARY(inv);



__SLL_OPERATOR_BINARY(shr);



__SLL_OPERATOR_BINARY(shl);



__SLL_OPERATOR_UNARY(len);



__SLL_OPERATOR_UNARY(access_zero);



__SLL_OPERATOR_BINARY(access);



__SLL_OPERATOR_TERNARY(access_range);



__SLL_OPERATOR_QUATERNARY(access_range_step);



__SLL_OPERATOR_BINARY(cast);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_bool_t sll_operator_bool(const sll_runtime_object_t* a);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_compare_result_t sll_operator_compare(const sll_runtime_object_t* a,const sll_runtime_object_t* b);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_bool_t sll_operator_equal(const sll_runtime_object_t* a,const sll_runtime_object_t* b);



#endif

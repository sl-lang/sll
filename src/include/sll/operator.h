#ifndef __SLL_OPERATOR_H__
#define __SLL_OPERATOR_H__ 1
#include <sll/common.h>
#include <sll/types.h>



#define __SLL_OPERATOR_UNARY(nm) __SLL_FUNC void sll_operator_##nm(const sll_runtime_object_t* restrict a,sll_runtime_object_t* restrict o)
#define __SLL_OPERATOR_BINARY(nm) __SLL_FUNC void sll_operator_##nm(const sll_runtime_object_t* restrict a,const sll_runtime_object_t* restrict b,sll_runtime_object_t* restrict o)
#define __SLL_OPERATOR_TERNARY(nm) __SLL_FUNC void sll_operator_##nm(const sll_runtime_object_t* restrict a,const sll_runtime_object_t* restrict b,const sll_runtime_object_t* restrict c,sll_runtime_object_t* restrict o)
#define __SLL_OPERATOR_QUATERNARY(nm) __SLL_FUNC void sll_operator_##nm(const sll_runtime_object_t* restrict a,const sll_runtime_object_t* restrict b,const sll_runtime_object_t* restrict c,const sll_runtime_object_t* restrict d,sll_runtime_object_t* restrict o)



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



__SLL_OPERATOR_UNARY(len);



__SLL_OPERATOR_UNARY(dup);



__SLL_OPERATOR_BINARY(access);



__SLL_OPERATOR_TERNARY(access_range);



__SLL_OPERATOR_QUATERNARY(access_range_step);



__SLL_FUNC __SLL_RETURN_COMPARE sll_operator_compare(const sll_runtime_object_t* a,const sll_runtime_object_t* b);



__SLL_FUNC __SLL_RETURN_COMPARE sll_operator_bool(const sll_runtime_object_t* a);



#endif

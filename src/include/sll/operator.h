#ifndef __SLL_OPERATOR_H__
#define __SLL_OPERATOR_H__ 1
#include <sll/common.h>
#include <sll/types.h>
#include <math.h>



#define __SLL_OPERATOR_UNARY(nm) __SLL_FUNC sll_runtime_object_t* sll_operator_##nm(const sll_runtime_object_t* a)
#define __SLL_OPERATOR_BINARY(nm) __SLL_FUNC sll_runtime_object_t* sll_operator_##nm(const sll_runtime_object_t* a,const sll_runtime_object_t* b)
#define __SLL_OPERATOR_TERNARY(nm) __SLL_FUNC sll_runtime_object_t* sll_operator_##nm(const sll_runtime_object_t* a,const sll_runtime_object_t* b,const sll_runtime_object_t* c)
#define __SLL_OPERATOR_QUATERNARY(nm) __SLL_FUNC sll_runtime_object_t* sll_operator_##nm(const sll_runtime_object_t* a,const sll_runtime_object_t* b,const sll_runtime_object_t* c,const sll_runtime_object_t* d)



#define SLL_ROUND_FLOAT(v) llround((v))



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



__SLL_OPERATOR_UNARY(dup);



__SLL_OPERATOR_BINARY(access);



__SLL_OPERATOR_TERNARY(access_range);



__SLL_OPERATOR_QUATERNARY(access_range_step);



__SLL_FUNC __SLL_RETURN_COMPARE sll_operator_compare(const sll_runtime_object_t* a,const sll_runtime_object_t* b);



__SLL_FUNC __SLL_RETURN_COMPARE sll_operator_bool(const sll_runtime_object_t* a);



#endif

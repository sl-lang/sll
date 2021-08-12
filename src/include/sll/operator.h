#ifndef __SLL_OPERATOR_H__
#define __SLL_OPERATOR_H__ 1
#include <sll/common.h>
#include <sll/types.h>



#define __SLL_OPERATOR_UNARY(nm) __SLL_FUNC void sll_operator_##nm(sll_runtime_object_t* a,sll_runtime_object_t* o)
#define __SLL_OPERATOR(nm) __SLL_FUNC void sll_operator_##nm(sll_runtime_object_t* a,sll_runtime_object_t* b,sll_runtime_object_t* o)



__SLL_OPERATOR_UNARY(inc);



__SLL_OPERATOR_UNARY(dec);



__SLL_OPERATOR(add);



__SLL_OPERATOR(sub);



__SLL_OPERATOR(mult);



__SLL_OPERATOR(div);



__SLL_OPERATOR(floor_div);



__SLL_OPERATOR(mod);



__SLL_OPERATOR(and);



__SLL_OPERATOR(or);



__SLL_OPERATOR(xor);



__SLL_OPERATOR_UNARY(inv);



#endif

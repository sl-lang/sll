#ifndef __SLL_STATIC_OBJECT_H__
#define __SLL_STATIC_OBJECT_H__ 1
#include <sll/gc.h>
#include <sll/types.h>



#define __SLL_STATIC_NAME(nm) sll_static_##nm
#define __SLL_STATIC_OBJECT_DECL(nm) extern sll_runtime_object_t* __SLL_STATIC_NAME(nm)



#define SLL_ACQUIRE_STATIC(nm) (SLL_ACQUIRE(__SLL_STATIC_NAME(nm)),__SLL_STATIC_NAME(nm))
#ifdef DEBUG_BUILD
#define SLL_FROM_INT(v) sll__add_debug_data(sll_int_to_object(v),__FILE__,__LINE__,__func__);
#define SLL_FROM_FLOAT(v) sll__add_debug_data(sll_float_to_object(v),__FILE__,__LINE__,__func__);
#else
#define SLL_FROM_INT(v) sll_int_to_object(v)
#define SLL_FROM_FLOAT(v) sll_float_to_object(v)
#endif
#define SLL_RETURN_ZERO return SLL_ACQUIRE_STATIC(int_zero)
#define SLL_RETURN_ONE return SLL_ACQUIRE_STATIC(int_one)
#define SLL_RETURN_ZERO_STRING return SLL_ACQUIRE_STATIC(str_zero)
#define SLL_RETURN_ZERO_HANDLE return SLL_ACQUIRE_STATIC(handle_zero);



__SLL_STATIC_OBJECT_DECL(int_minus_one);



__SLL_STATIC_OBJECT_DECL(int_zero);



__SLL_STATIC_OBJECT_DECL(int_one);



__SLL_STATIC_OBJECT_DECL(int_two);



__SLL_STATIC_OBJECT_DECL(int_three);



__SLL_STATIC_OBJECT_DECL(int_four);



__SLL_STATIC_OBJECT_DECL(float_zero);



__SLL_STATIC_OBJECT_DECL(float_one);



__SLL_STATIC_OBJECT_DECL(float_half);



__SLL_STATIC_OBJECT_DECL(float_third);



__SLL_STATIC_OBJECT_DECL(str_zero);



__SLL_STATIC_OBJECT_DECL(array_zero);



__SLL_STATIC_OBJECT_DECL(handle_zero);



__SLL_STATIC_OBJECT_DECL(map_zero);



__SLL_FUNC sll_runtime_object_t* sll_int_to_object(sll_integer_t v);



__SLL_FUNC sll_runtime_object_t* sll_float_to_object(sll_float_t v);



#endif

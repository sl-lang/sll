#ifndef __SLL_STATIC_OBJECT_H__
#define __SLL_STATIC_OBJECT_H__ 1
#include <sll/gc.h>
#include <sll/types.h>



#define __SLL_STATIC_NAME(nm) sll_static_##nm
#define __SLL_STATIC_OBJECT_DECL(nm) extern sll_runtime_object_t* __SLL_STATIC_NAME(nm)



#define SLL_ACQUIRE_STATIC(nm) (SLL_ACQUIRE(__SLL_STATIC_NAME(nm)),__SLL_STATIC_NAME(nm))
#define SLL_ACQUIRE_STATIC_INT(v) (SLL_ACQUIRE(((v)<0?sll_static_negative_int[-(v)-1]:sll_static_int[(v)])),((v)<0?sll_static_negative_int[-(v)-1]:sll_static_int[(v)]))
#define SLL_ACQUIRE_STATIC_CHAR(c) SLL_FROM_CHAR(c)
#define SLL_FROM_INT(v) __SLL_ADD_DEBUG_DATA(sll_int_to_object(v),__SLL_DEBUG_TYPE_CREATE)
#define SLL_FROM_FLOAT(v) __SLL_ADD_DEBUG_DATA(sll_float_to_object(v),__SLL_DEBUG_TYPE_CREATE)
#define SLL_FROM_CHAR(c) __SLL_ADD_DEBUG_DATA(sll_static_char[(sll_char_t)(c)],__SLL_DEBUG_TYPE_CREATE)
#define SLL_RETURN_ZERO return SLL_ACQUIRE_STATIC_INT(0)
#define SLL_RETURN_ONE return SLL_ACQUIRE_STATIC_INT(1)
#define SLL_RETURN_ZERO_STRING return SLL_ACQUIRE_STATIC(str_zero)
#define SLL_RETURN_ZERO_HANDLE return SLL_ACQUIRE_STATIC(handle_zero)



extern sll_runtime_object_t* sll_static_int[256];



extern sll_runtime_object_t* sll_static_negative_int[16];



__SLL_STATIC_OBJECT_DECL(float_zero);



__SLL_STATIC_OBJECT_DECL(float_one);



__SLL_STATIC_OBJECT_DECL(float_half);



extern sll_runtime_object_t* sll_static_char[256];



__SLL_STATIC_OBJECT_DECL(str_zero);



__SLL_STATIC_OBJECT_DECL(array_zero);



__SLL_STATIC_OBJECT_DECL(handle_zero);



__SLL_STATIC_OBJECT_DECL(map_zero);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_int_to_object(sll_integer_t v);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_float_to_object(sll_float_t v);



#endif

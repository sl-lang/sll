#ifndef __SLL_STATIC_OBJECT_H__
#define __SLL_STATIC_OBJECT_H__ 1
#include <sll/gc.h>
#include <sll/types.h>



#define SLL_ACQUIRE_STATIC(nm) (SLL_ACQUIRE(sll_static_##nm),sll_static_##nm)
#define SLL_ACQUIRE_STATIC_INT(v) (SLL_ACQUIRE(((v)<0?sll_static_negative_int[-(v)-1]:sll_static_int[(v)])),((v)<0?sll_static_negative_int[-(v)-1]:sll_static_int[(v)]))
#define SLL_ACQUIRE_STATIC_CHAR(c) SLL_FROM_CHAR(c)
#define SLL_FROM_INT(v) __SLL_ADD_DEBUG_DATA(sll_int_to_object(v),__SLL_DEBUG_TYPE_CREATE)
#define SLL_FROM_FLOAT(v) __SLL_ADD_DEBUG_DATA(sll_float_to_object(v),__SLL_DEBUG_TYPE_CREATE)
#define SLL_FROM_CHAR(c) (__SLL_ADD_DEBUG_DATA(sll_static_char[(sll_char_t)(c)],__SLL_DEBUG_TYPE_CREATE)->rc++,sll_static_char[(sll_char_t)(c)])



extern sll_runtime_object_t* sll_static_int[256];



extern sll_runtime_object_t* sll_static_negative_int[16];



extern sll_runtime_object_t* sll_static_float_zero;



extern sll_runtime_object_t* sll_static_float_one;



extern sll_runtime_object_t* sll_static_float_half;



extern sll_runtime_object_t* sll_static_char[256];



extern sll_runtime_object_t* sll_static_str_zero;



extern sll_runtime_object_t* sll_static_array_zero;



extern sll_runtime_object_t* sll_static_handle_zero;



extern sll_runtime_object_t* sll_static_map_zero;



__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_int_to_object(sll_integer_t v);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_float_to_object(sll_float_t v);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_char_to_object(sll_char_t v);



#endif

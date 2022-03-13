#ifndef __SLL_INTERNAL_STATIC_OBJECT_H__
#define __SLL_INTERNAL_STATIC_OBJECT_H__ 1
#include <sll/common.h>
#include <sll/object.h>
#include <sll/types.h>



#define __SLL_STATIC_INT_OBJECT(v) static sll_object_t _int_##v##_static_data={1,SLL_OBJECT_TYPE_INT,.dt={.i=(v)}}
#define __SLL_STATIC_NEG_INT_OBJECT(v) static sll_object_t _int_neg_##v##_static_data={1,SLL_OBJECT_TYPE_INT,.dt={.i=-(v)}}
#define __SLL_STATIC_CHAR_OBJECT(v) static sll_object_t _char_##v##_static_data={1,SLL_OBJECT_TYPE_CHAR,.dt={.c=(sll_char_t)(v)}}
#define __SLL_STATIC_OBJECT(nm,t,f,v) static sll_object_t _##nm##_static_data={1,t,.dt={.f=v}};__SLL_EXTERNAL sll_object_t* sll_static_##nm=&_##nm##_static_data



#endif

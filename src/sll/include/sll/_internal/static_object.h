#ifndef __SLL__INTERNAL_STATIC_OBJECT_H__
#define __SLL__INTERNAL_STATIC_OBJECT_H__ 1
#include <sll/_internal/common.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/object.h>
#include <sll/string.h>
#include <sll/types.h>



#define STATIC_INT_OBJECT(v) static sll_object_t _int_##v##_static_data={1,SLL_OBJECT_TYPE_INT,.dt={.i=(v)}}
#define STATIC_NEG_INT_OBJECT(v) static sll_object_t _int_neg_##v##_static_data={1,SLL_OBJECT_TYPE_INT,.dt={.i=-(v)}}
#define STATIC_CHAR_OBJECT(v) static sll_object_t _char_##v##_static_data={1,SLL_OBJECT_TYPE_CHAR,.dt={.c=(sll_char_t)(v)}}
#define STATIC_OBJECT(nm,t,f,v) static sll_object_t _##nm##_static_data={1,t,.dt={.f=v}};__SLL_EXTERNAL sll_object_t* sll_static_##nm=&_##nm##_static_data



static __SLL_FORCE_INLINE sll_object_t* STRING_TO_OBJECT(const sll_string_t* v){
	sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
	if (v){
		sll_string_clone(v,&(o->dt.s));
	}
	else{
		SLL_INIT_STRING(&(o->dt.s));
	}
	return o;
}



static __SLL_FORCE_INLINE sll_object_t* STRING_TO_OBJECT_NOCOPY(sll_string_t* v){
	SLL_ASSERT(v);
	sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
	o->dt.s=*v;
	return o;
}



#endif

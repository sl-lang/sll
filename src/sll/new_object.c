#include <sll/_internal/common.h>
#include <sll/_size_types.h>
#include <sll/allocator.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/map.h>
#include <sll/memory.h>
#include <sll/new_object.h>
#include <sll/operator.h>
#include <sll/object.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/var_arg.h>
#include <stdarg.h>



#define BUILD_CLONE_TYPE(type) \
	do{ \
		sll_object_t* obj=sll_var_arg_get_object(va); \
		sll_object_t* o=(SLL_OBJECT_GET_TYPE(obj)==(type)?sll_operator_copy(obj,0):sll_operator_cast(obj,sll_static_int[(type)])); \
		sll_release_object(obj); \
		return o; \
	} while (0)

#define SKIP_WHITESPACE \
	while (*tl&&(**t==' '||(**t>8&&**t<14))){ \
		(*tl)--; \
		(*t)++; \
	}



static sll_object_t* _build_single(const sll_char_t** t,sll_string_length_t* tl,sll_var_arg_list_t* va){
	SLL_ASSERT(va->t==SLL_VAR_ARG_LIST_TYPE_C);
	SKIP_WHITESPACE;
	if (!(*tl)){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	(*tl)--;
	sll_char_t st=**t;
	(*t)++;
	switch (st){
		case '1':
			return SLL_ACQUIRE_STATIC_INT(1);
		case 'h':
			return sll_int_to_object((__SLL_S32)sll_var_arg_get_int(va));
		case 'i':
			return sll_int_to_object(sll_var_arg_get_int(va));
		case 'f':
			return sll_float_to_object(sll_var_arg_get_float(va));
		case 'c':
			return sll_char_to_object(sll_var_arg_get_char(va));
		case 's':
			return sll_string_to_object(sll_var_arg_get(va));
		case 'S':
			{
				const sll_char_t* ptr=sll_var_arg_get(va);
				return (ptr?sll_string_to_object_pointer(ptr,SLL_MAX_STRING_LENGTH):sll_string_to_object(NULL));
			}
		case 'L':
			{
				const sll_char_t* ptr=sll_var_arg_get(va);
				sll_string_length_t len=(sll_string_length_t)sll_var_arg_get_int(va);
				return (ptr&&len?sll_string_to_object_pointer(ptr,len):sll_string_to_object(NULL));
			}
		case 'Z':
			return sll_string_to_object(NULL);
		case 'a':
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				const sll_array_t* ptr=sll_var_arg_get(va);
				if (!ptr){
					sll_array_create(0,&(o->dt.a));
				}
				else{
					sll_array_clone(ptr,&(o->dt.a));
				}
				return o;
			}
		case 'A':
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_array_create(0,&(o->dt.a));
				return o;
			}
		case 'm':
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				const sll_map_t* ptr=sll_var_arg_get(va);
				if (!ptr){
					sll_map_create(0,&(o->dt.m));
				}
				else{
					sll_map_clone(ptr,&(o->dt.m));
				}
				return o;
			}
		case 'M':
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_create(0,&(o->dt.m));
				return o;
			}
		case '[':
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_array_create(0,&(o->dt.a));
				SKIP_WHITESPACE;
				while (tl&&**t!=']'){
					o->dt.a.l++;
					sll_allocator_resize((void**)(&(o->dt.a.v)),o->dt.a.l*sizeof(sll_object_t*));
					o->dt.a.v[o->dt.a.l-1]=_build_single(t,tl,va);
					SKIP_WHITESPACE;
				}
				if (tl){
					(*tl)--;
					(*t)++;
				}
				return o;
			}
		case '<':
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_create(0,&(o->dt.m));
				sll_bool_t val=0;
				SKIP_WHITESPACE;
				while (tl&&**t!='>'){
					if (!val){
						o->dt.m.l++;
						o->dt.m.v=sll_reallocate(o->dt.m.v,(o->dt.m.l<<1)*sizeof(sll_object_t*));
					}
					o->dt.m.v[((o->dt.m.l-1)<<1)+val]=_build_single(t,tl,va);
					val=!val;
					SKIP_WHITESPACE;
				}
				if (val){
					o->dt.m.v[(o->dt.m.l<<1)-1]=SLL_ACQUIRE_STATIC_INT(0);
				}
				if (tl){
					(*tl)--;
					(*t)++;
				}
				return o;
			}
		case 'O':
		case 'N':
			{
				sll_object_t* o=sll_var_arg_get_object(va);
				if (st=='N'){
					sll_release_object(o);
				}
				return o;
			}
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



static sll_object_t* _build_single_sll(const sll_char_t** t,sll_string_length_t* tl,sll_var_arg_list_t* va){
	SLL_ASSERT(va->t==SLL_VAR_ARG_LIST_TYPE_SLL);
	SKIP_WHITESPACE;
	if (!(*tl)){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	(*tl)--;
	sll_char_t st=**t;
	(*t)++;
	switch (st){
		case '1':
			return SLL_ACQUIRE_STATIC_INT(1);
		case 'i':
			return sll_int_to_object(sll_var_arg_get_int(va));
		case 'f':
			return sll_float_to_object(sll_var_arg_get_float(va));
		case 'c':
			return sll_char_to_object(sll_var_arg_get_char(va));
		case 's':
			BUILD_CLONE_TYPE(SLL_OBJECT_TYPE_STRING);
		case 'Z':
			return sll_string_to_object(NULL);
		case 'a':
			BUILD_CLONE_TYPE(SLL_OBJECT_TYPE_ARRAY);
		case 'A':
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_array_create(0,&(o->dt.a));
				return o;
			}
		case 'm':
			BUILD_CLONE_TYPE(SLL_OBJECT_TYPE_MAP);
		case 'M':
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_create(0,&(o->dt.m));
				return o;
			}
		case '[':
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
				sll_array_create(0,&(o->dt.a));
				SKIP_WHITESPACE;
				while (tl&&**t!=']'){
					o->dt.a.l++;
					sll_allocator_resize((void**)(&(o->dt.a.v)),o->dt.a.l*sizeof(sll_object_t*));
					o->dt.a.v[o->dt.a.l-1]=_build_single_sll(t,tl,va);
					SKIP_WHITESPACE;
				}
				if (tl){
					(*tl)--;
					(*t)++;
				}
				return o;
			}
		case '<':
			{
				sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_MAP);
				sll_map_create(0,&(o->dt.m));
				sll_bool_t val=0;
				SKIP_WHITESPACE;
				while (tl&&**t!='>'){
					if (!val){
						o->dt.m.l++;
						o->dt.m.v=sll_reallocate(o->dt.m.v,(o->dt.m.l<<1)*sizeof(sll_object_t*));
					}
					o->dt.m.v[((o->dt.m.l-1)<<1)+val]=_build_single_sll(t,tl,va);
					val=!val;
					SKIP_WHITESPACE;
				}
				if (val){
					o->dt.m.v[(o->dt.m.l<<1)-1]=SLL_ACQUIRE_STATIC_INT(0);
				}
				if (tl){
					(*tl)--;
					(*t)++;
				}
				return o;
			}
		case 'O':
			return sll_var_arg_get_object(va);
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_new_object(const sll_char_t* t,...){
	va_list va;
	va_start(va,t);
	sll_var_arg_list_t dt={
		SLL_VAR_ARG_LIST_TYPE_C,
		{
			.c=&va
		}
	};
	sll_object_t* o=sll_new_object_list(t,sll_string_length_unaligned(t),&dt);
	va_end(va);
	return o;
}



__SLL_EXTERNAL void sll_new_object_array(const sll_char_t* t,sll_array_t* o,...){
	sll_string_length_t tl=sll_string_length_unaligned(t);
	sll_array_create(0,o);
	if (!tl){
		return;
	}
	va_list va;
	va_start(va,o);
	sll_var_arg_list_t dt={
		SLL_VAR_ARG_LIST_TYPE_C,
		{
			.c=&va
		}
	};
	while (tl){
		o->l++;
		sll_allocator_resize((void**)(&(o->v)),o->l*sizeof(sll_object_t*));
		o->v[o->l-1]=_build_single(&t,&tl,&dt);
	}
	va_end(va);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_new_object_list(const sll_char_t* t,sll_string_length_t tl,sll_var_arg_list_t* va){
	if (!tl){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_t* (*fn)(const sll_char_t**,sll_string_length_t*,sll_var_arg_list_t*)=(va->t==SLL_VAR_ARG_LIST_TYPE_C?_build_single:_build_single_sll);
	sll_object_t* e=fn(&t,&tl,va);
	if (!tl){
		return e;
	}
	sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
	sll_array_create(1,&(o->dt.a));
	o->dt.a.v[0]=e;
	do{
		o->dt.a.l++;
		sll_allocator_resize((void**)(&(o->dt.a.v)),o->dt.a.l*sizeof(sll_object_t*));
		o->dt.a.v[o->dt.a.l-1]=fn(&t,&tl,va);
	} while (tl);
	return o;
}

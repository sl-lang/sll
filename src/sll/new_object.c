#include <sll/_internal/common.h>
#include <sll/_internal/gc.h>
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
		sll_object_t* o=(obj->t==(type)?sll_operator_copy(obj,0):sll_operator_cast(obj,sll_static_int[(type)])); \
		GC_RELEASE(obj); \
		return o; \
	} while (0)

#define SKIP_WHITESPACE \
	while (*tl&&(**t==' '||(**t>8&&**t<14))){ \
		(*tl)--; \
		(*t)++; \
	}



static sll_object_t* _build_single(const sll_char_t** t,sll_string_length_t* tl,sll_var_arg_list_t* va){
	SKIP_WHITESPACE;
	if (!(*tl)){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	(*tl)--;
	sll_char_t st=**t;
	(*t)++;
	if (va->t==SLL_VAR_ARG_LIST_TYPE_C){
		switch (st){
			case 'h':
				return sll_int_to_object((__SLL_S32)sll_var_arg_get_int(va));
			case 'u':
				return sll_int_to_object((__SLL_U32)sll_var_arg_get_int(va));
			case 's':
				return sll_string_to_object(sll_var_arg_get(va));
			case 'S':
				{
					const sll_char_t* ptr=sll_var_arg_get(va);
					return (ptr?sll_string_pointer_to_object(ptr,SLL_MAX_STRING_LENGTH):sll_string_to_object(NULL));
				}
			case 'l':
				{
					const sll_char_t* ptr=sll_var_arg_get(va);
					sll_string_length_t len=(sll_string_length_t)sll_var_arg_get_int(va);
					return (ptr&&len?sll_string_pointer_to_object(ptr,len):sll_string_to_object(NULL));
				}
			case 'a':
				return sll_array_to_object(sll_var_arg_get(va));
			case 'L':
			case 'n':
				{
					sll_object_t*const* ptr=(sll_object_t*const*)sll_var_arg_get(va);
					sll_array_length_t len=(sll_array_length_t)sll_var_arg_get_int(va);
					if (!ptr||!len){
						return sll_array_to_object(NULL);
					}
					sll_object_t* o=sll_array_length_to_object(len);
					while (len){
						len--;
						if (st=='L'){
							SLL_ACQUIRE(*(ptr+len));
						}
						o->dt.a.v[len]=*(ptr+len);
					}
					return o;
				}
			case 'x':
				{
					const sll_char_t*const* ptr=(const sll_char_t*const*)sll_var_arg_get(va);
					sll_array_length_t len=(sll_array_length_t)sll_var_arg_get_int(va);
					if (!ptr||!len){
						return sll_array_to_object(NULL);
					}
					sll_object_t* o=sll_array_length_to_object(len);
					while (len){
						len--;
						o->dt.a.v[len]=sll_string_pointer_to_object(*(ptr+len),SLL_MAX_STRING_LENGTH);
					}
					return o;
				}
			case 'm':
				return sll_map_to_object(sll_var_arg_get(va));
			case 'N':
				{
					sll_object_t* o=sll_var_arg_get_object(va);
					GC_RELEASE(o);
					return o;
				}
		}
	}
	switch (st){
		case '1':
			return SLL_ACQUIRE_STATIC_INT(1);
		case 'i':
			return sll_int_to_object(sll_var_arg_get_int(va));
		case 'f':
			return sll_float_to_object(sll_var_arg_get_float(va));
		case 'c':
			return sll_char_to_object(sll_var_arg_get_char(va));
		case 'C':
			return sll_char_to_string_object(sll_var_arg_get_char(va));
		case 's':
			BUILD_CLONE_TYPE(SLL_OBJECT_TYPE_STRING);
		case 'z':
			return SLL_FROM_CHAR(0);
		case 'Z':
			return sll_string_to_object(NULL);
		case 'a':
			BUILD_CLONE_TYPE(SLL_OBJECT_TYPE_ARRAY);
		case 'A':
			return sll_array_to_object(NULL);
		case 'm':
			BUILD_CLONE_TYPE(SLL_OBJECT_TYPE_MAP);
		case 'M':
			return sll_map_to_object(NULL);
		case '(':
		case '[':
			{
				sll_char_t ec=(st=='('?')':']');
				sll_object_t* o=sll_array_to_object(NULL);
				SKIP_WHITESPACE;
				while (*tl&&**t!=ec){
					o->dt.a.l++;
					sll_allocator_resize((void**)(&(o->dt.a.v)),o->dt.a.l*sizeof(sll_object_t*));
					o->dt.a.v[o->dt.a.l-1]=_build_single(t,tl,va);
					SKIP_WHITESPACE;
				}
				if (st=='('){
					sll_allocator_collapse((void**)(&(o->dt.a.v)),o->dt.a.l*sizeof(sll_object_t*));
				}
				if (*tl){
					(*tl)--;
					(*t)++;
				}
				return o;
			}
		case '<':
			{
				sll_object_t* o=sll_map_to_object(NULL);
				sll_bool_t val=0;
				SKIP_WHITESPACE;
				while (*tl&&**t!='>'){
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
				if (*tl){
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
	va_list va;
	va_start(va,o);
	sll_var_arg_list_t dt={
		SLL_VAR_ARG_LIST_TYPE_C,
		{
			.c=&va
		}
	};
	sll_new_object_array_list(t,sll_string_length_unaligned(t),&dt,o);
	va_end(va);
}



__SLL_EXTERNAL void sll_new_object_array_list(const sll_char_t* t,sll_string_length_t tl,sll_var_arg_list_t* va,sll_array_t* o){
	sll_array_create(0,o);
	if (!tl){
		return;
	}
	while (tl){
		o->l++;
		sll_allocator_resize((void**)(&(o->v)),o->l*sizeof(sll_object_t*));
		o->v[o->l-1]=_build_single(&t,&tl,va);
	}
	sll_allocator_collapse((void**)(&(o->v)),o->l*sizeof(sll_object_t*));
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_new_object_list(const sll_char_t* t,sll_string_length_t tl,sll_var_arg_list_t* va){
	if (!tl){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_t* e=_build_single(&t,&tl,va);
	if (!tl){
		return e;
	}
	sll_object_t* o=sll_array_length_to_object(1);
	o->dt.a.v[0]=e;
	do{
		o->dt.a.l++;
		sll_allocator_resize((void**)(&(o->dt.a.v)),o->dt.a.l*sizeof(sll_object_t*));
		o->dt.a.v[o->dt.a.l-1]=_build_single(&t,&tl,va);
	} while (tl);
	sll_allocator_collapse((void**)(&(o->dt.a.v)),o->dt.a.l*sizeof(sll_object_t*));
	return o;
}

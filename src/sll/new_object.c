#include <sll/_internal/common.h>
#include <sll/_internal/new_object.h>
#include <sll/_internal/static_object.h>
#include <sll/_internal/var_arg.h>
#include <sll/_size_types.h>
#include <sll/allocator.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/log.h>
#include <sll/map.h>
#include <sll/memory.h>
#include <sll/new_object.h>
#include <sll/object.h>
#include <sll/operator.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/var_arg.h>
#include <stdarg.h>



#define BUILD_CLONE_TYPE(type) \
	do{ \
		if (fl&NEW_OBJECT_FLAG_ARRAY){ \
			SLL_UNIMPLEMENTED(); \
		} \
		sll_object_t* obj=sll_var_arg_get_object(va); \
		sll_object_t* o=(obj->t==(type)?sll_operator_copy(obj,0):sll_operator_cast(obj,sll_static_int[(type)])); \
		SLL_RELEASE(obj); \
		return o; \
	} while (0)

#define SKIP_MODIFIERS \
	while (tl&&(*t==' '||(*t>8&&*t<14)||*t=='!'||*t=='+'||*t=='*')){ \
		tl--; \
		t++; \
	}
#define SKIP_WHITESPACE_VAR(t,tl) \
	while ((tl)&&((*t)==' '||((*t)>8&&(*t)<14))){ \
		(tl)--; \
		(t)++; \
	}
#define SKIP_WHITESPACE SKIP_WHITESPACE_VAR(*t,*tl)



static sll_object_t* _build_single(const sll_char_t** t,sll_string_length_t* tl,sll_var_arg_list_t* va);



static void _build_struct_offsets(const sll_char_t** t,sll_string_length_t* tl,sll_var_arg_list_t* va,struct_offset_builder_t* o){
	SKIP_WHITESPACE;
	if (!(*tl)){
		return;
	}
	(*tl)--;
	sll_char_t st=**t;
	(*t)++;
	sll_arg_count_t cnt=1;
	while (*tl&&(**t==' '||(**t>8&&**t<14)||**t=='!'||**t=='+'||**t=='*')){
		if (**t=='+'){
			cnt=2;
		}
		(*tl)--;
		(*t)++;
	}
	switch (st){
		case '#':
			o->fnl++;
			o->fn=sll_reallocate(o->fn,o->fnl*sizeof(void*));
			*(o->fn+o->fnl-1)=sll_var_arg_get(va);
		case 'p':
			cnt=1;
		case 'h':
		case 'u':
		case 'i':
		case 'f':
		case 'c':
		case 'C':
		case 'd':
		case 's':
		case 'S':
		case 'l':
		case 'a':
		case 'm':
		case 'O':
			o->l+=cnt;
			o->off=sll_reallocate(o->off,o->l*sizeof(sll_size_t));
			*(o->off+o->l-1)=sll_var_arg_get_int(va);
			if (cnt==2){
				*(o->off+o->l-2)=sll_var_arg_get_int(va);
			}
			return;
		case '(':
		case '[':
		case '<':
			{
				sll_char_t ec=(st=='('?')':(st=='['?']':'>'));
				SKIP_WHITESPACE;
				while (*tl&&**t!=ec){
					_build_struct_offsets(t,tl,va,o);
					SKIP_WHITESPACE;
				}
				if (*tl){
					(*tl)--;
					(*t)++;
				}
				return;
			}
	}
}



static sll_object_t* _build_struct(const sll_char_t** t,sll_string_length_t* tl,sll_var_arg_list_t* va,object_creation_flags_t fl){
	addr_t ptr=sll_var_arg_get_int(va);
	sll_array_length_t len=(sll_array_length_t)sll_var_arg_get_int(va);
	sll_size_t sz=sll_var_arg_get_int(va);
	SKIP_WHITESPACE;
	const sll_char_t* base_t=*t;
	sll_string_length_t base_tl=*tl;
	struct_offset_builder_t off_dt={
		NULL,
		0,
		NULL,
		0
	};
	while (*tl&&**t!='}'){
		_build_struct_offsets(t,tl,va,&off_dt);
		SKIP_WHITESPACE;
	}
	if (*tl){
		(*tl)--;
		(*t)++;
	}
	sll_object_t* o=sll_array_length_to_object(len);
	sll_bool_t deref=0;
	if (!sz){
		sz=sizeof(const void*);
		deref=1;
	}
	ptr+=len*sz;
	while (len){
		len--;
		ptr-=sz;
		const sll_char_t* arg_t=base_t;
		sll_string_length_t arg_tl=base_tl;
		sll_var_arg_list_t arg_va={
			VAR_ARG_LIST_TYPE_STRUCT,
			{
				.s={
					(deref?*((const void**)ptr):(const void*)ptr),
					off_dt.off,
					off_dt.l,
					off_dt.fn,
					off_dt.fnl
				}
			}
		};
		sll_object_t* arg=sll_array_to_object(NULL);
		o->dt.array.data[len]=arg;
		while (arg_tl&&*arg_t!='}'){
			arg->dt.array.length++;
			sll_allocator_resize((void**)(&(arg->dt.array.data)),arg->dt.array.length*sizeof(sll_object_t*));
			arg->dt.array.data[arg->dt.array.length-1]=_build_single(&arg_t,&arg_tl,&arg_va);
			SKIP_WHITESPACE_VAR(arg_t,arg_tl);
		}
		sll_allocator_collapse((void**)(&(arg->dt.array.data)),arg->dt.array.length*sizeof(sll_object_t*));
	}
	sll_deallocate(off_dt.off);
	sll_deallocate(off_dt.fn);
	return o;
}



static sll_object_t* _build_single(const sll_char_t** t,sll_string_length_t* tl,sll_var_arg_list_t* va){
	SKIP_WHITESPACE;
	if (!(*tl)){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	const sll_char_t* src_t=*t;
	(*tl)--;
	sll_char_t st=**t;
	(*t)++;
	object_creation_flags_t fl=0;
	while (1){
		SKIP_WHITESPACE;
		if (**t=='!'){
			fl|=NEW_OBJECT_FLAG_NO_ACQUIRE;
		}
		else if (**t=='+'){
			fl|=NEW_OBJECT_FLAG_ARRAY;
		}
		else if (**t=='*'){
			fl|=NEW_OBJECT_FLAG_DEREF;
		}
		else{
			break;
		}
		(*t)++;
		(*tl)--;
	}
	if (va->t!=SLL_VAR_ARG_LIST_TYPE_SLL){
		switch (st){
			case 'h':
				if (fl&NEW_OBJECT_FLAG_ARRAY){
					SLL_UNIMPLEMENTED();
				}
				return sll_int_to_object((__SLL_S32)sll_var_arg_get_int(va));
			case 'u':
				if (fl&NEW_OBJECT_FLAG_ARRAY){
					SLL_UNIMPLEMENTED();
				}
				return sll_int_to_object((__SLL_U32)sll_var_arg_get_int(va));
			case 'p':
				return sll_int_to_object(_var_arg_get_pointer(va));
			case 's':
				{
					if (!(fl&NEW_OBJECT_FLAG_ARRAY)){
						sll_string_t* ptr=PTR(_var_arg_get_pointer(va));
						return (ptr?STRING_TO_OBJECT(ptr):EMPTY_STRING_TO_OBJECT());
					}
					const void* ptr=PTR(_var_arg_get_pointer(va));
					sll_array_length_t len=(sll_array_length_t)sll_var_arg_get_int(va);
					if (!ptr||!len){
						return sll_array_to_object(NULL);
					}
					sll_object_t* o=sll_array_length_to_object(len);
					while (len){
						len--;
						o->dt.array.data[len]=STRING_TO_OBJECT(((fl&NEW_OBJECT_FLAG_DEREF)?*(((const sll_string_t*const*)ptr)+len):((const sll_string_t*)ptr)+len));
					}
					return o;
				}
			case 'S':
				{
					if (!(fl&NEW_OBJECT_FLAG_ARRAY)){
						const sll_char_t* ptr=PTR(_var_arg_get_pointer(va));
						return (ptr?STRING_POINTER_TO_OBJECT(ptr):EMPTY_STRING_TO_OBJECT());
					}
					const sll_char_t*const* ptr=PTR(_var_arg_get_pointer(va));
					sll_array_length_t len=(sll_array_length_t)sll_var_arg_get_int(va);
					if (!ptr||!len){
						return sll_array_to_object(NULL);
					}
					sll_object_t* o=sll_array_length_to_object(len);
					while (len){
						len--;
						o->dt.array.data[len]=(*(ptr+len)?STRING_POINTER_TO_OBJECT(*(ptr+len)):EMPTY_STRING_TO_OBJECT());
					}
					return o;
				}
			case 'l':
				{
					if (fl&NEW_OBJECT_FLAG_ARRAY){
						SLL_UNIMPLEMENTED();
					}
					const sll_char_t* ptr=sll_var_arg_get(va);
					sll_string_length_t len=(sll_string_length_t)sll_var_arg_get_int(va);
					return (ptr&&len?STRING_POINTER_LENGTH_TO_OBJECT(ptr,len):EMPTY_STRING_TO_OBJECT());
				}
			case 'a':
				if (fl&NEW_OBJECT_FLAG_ARRAY){
					SLL_UNIMPLEMENTED();
				}
				return sll_array_to_object(sll_var_arg_get(va));
			case 'm':
				if (fl&NEW_OBJECT_FLAG_ARRAY){
					SLL_UNIMPLEMENTED();
				}
				return sll_map_to_object(sll_var_arg_get(va));
			case '#':
				return _var_arg_converter(va);
			case '{':
				return _build_struct(t,tl,va,fl);
		}
	}
	else{
		fl=0;
	}
	switch (st){
		case '1':
			return SLL_ACQUIRE_STATIC_INT(1);
		case 'i':
			if (fl&NEW_OBJECT_FLAG_ARRAY){
				SLL_UNIMPLEMENTED();
			}
			return sll_int_to_object(sll_var_arg_get_int(va));
		case 'f':
			if (fl&NEW_OBJECT_FLAG_ARRAY){
				SLL_UNIMPLEMENTED();
			}
			return sll_float_to_object(sll_var_arg_get_float(va));
		case 'c':
			if (fl&NEW_OBJECT_FLAG_ARRAY){
				SLL_UNIMPLEMENTED();
			}
			return sll_char_to_object(sll_var_arg_get_char(va));
		case 'C':
			if (fl&NEW_OBJECT_FLAG_ARRAY){
				SLL_UNIMPLEMENTED();
			}
			return sll_char_to_string_object(sll_var_arg_get_char(va));
		case 'd':
			{
				if (fl&NEW_OBJECT_FLAG_ARRAY){
					SLL_UNIMPLEMENTED();
				}
				sll_complex_t n;
				sll_var_arg_get_complex(va,&n);
				return sll_complex_to_object(&n);
			}
		case 's':
			BUILD_CLONE_TYPE(SLL_OBJECT_TYPE_STRING);
		case 'z':
			return SLL_FROM_CHAR(0);
		case 'Z':
			return EMPTY_STRING_TO_OBJECT();
		case 'a':
			BUILD_CLONE_TYPE(SLL_OBJECT_TYPE_ARRAY);
		case 'A':
			return sll_array_to_object(NULL);
		case 'm':
			BUILD_CLONE_TYPE(SLL_OBJECT_TYPE_MAP);
		case 'M':
			return sll_map_to_object(NULL);
		case 'O':
			{
				if (fl&NEW_OBJECT_FLAG_ARRAY){
					sll_object_t*const* ptr=sll_var_arg_get(va);
					sll_array_length_t len=(sll_array_length_t)sll_var_arg_get_int(va);
					if (!ptr||!len){
						return sll_array_to_object(NULL);
					}
					sll_object_t* o=sll_array_length_to_object(len);
					while (len){
						len--;
						if (!(fl&NEW_OBJECT_FLAG_NO_ACQUIRE)){
							SLL_ACQUIRE(*(ptr+len));
						}
						o->dt.array.data[len]=*(ptr+len);
					}
					return o;
				}
				sll_object_t* o=sll_var_arg_get_object(va);
				if (fl&NEW_OBJECT_FLAG_NO_ACQUIRE){
					SLL_RELEASE(o);
				}
				return o;
			}
		case '(':
		case '[':
			{
				sll_char_t ec=(st=='('?')':']');
				sll_object_t* o=sll_array_to_object(NULL);
				SKIP_WHITESPACE;
				while (*tl&&**t!=ec){
					o->dt.array.length++;
					sll_allocator_resize((void**)(&(o->dt.array.data)),o->dt.array.length*sizeof(sll_object_t*));
					o->dt.array.data[o->dt.array.length-1]=_build_single(t,tl,va);
					SKIP_WHITESPACE;
				}
				if (st=='('){
					sll_allocator_collapse((void**)(&(o->dt.array.data)),o->dt.array.length*sizeof(sll_object_t*));
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
						o->dt.map.length++;
						o->dt.map.data=sll_reallocate(o->dt.map.data,(o->dt.map.length<<1)*sizeof(sll_object_t*));
					}
					o->dt.map.data[((o->dt.map.length-1)<<1)+val]=_build_single(t,tl,va);
					val=!val;
					SKIP_WHITESPACE;
				}
				if (val){
					o->dt.map.data[(o->dt.map.length<<1)-1]=SLL_ACQUIRE_STATIC_INT(0);
				}
				if (*tl){
					(*tl)--;
					(*t)++;
				}
				return o;
			}
	}
	if (st!='0'){
		SLL_WARN("Ignoring unknown format code '%c': %s",st,src_t);
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_new_object(const sll_char_t* t,...){
	va_list va;
	va_start(va,t);
	sll_var_arg_list_t dt;
	SLL_VAR_ARG_INIT_C(&dt,&va);
	sll_object_t* o=sll_new_object_list(t,sll_string_length(t),&dt);
	va_end(va);
	return o;
}



__SLL_EXTERNAL void sll_new_object_array(const sll_char_t* t,sll_array_t* o,...){
	va_list va;
	va_start(va,o);
	sll_var_arg_list_t dt;
	SLL_VAR_ARG_INIT_C(&dt,&va);
	sll_new_object_array_list(t,sll_string_length(t),&dt,o);
	va_end(va);
}



__SLL_EXTERNAL void sll_new_object_array_list(const sll_char_t* t,sll_string_length_t tl,sll_var_arg_list_t* va,sll_array_t* o){
	sll_array_create(0,o);
	SKIP_MODIFIERS;
	if (!tl){
		return;
	}
	while (tl){
		o->length++;
		sll_allocator_resize((void**)(&(o->data)),o->length*sizeof(sll_object_t*));
		o->data[o->length-1]=_build_single(&t,&tl,va);
	}
	sll_allocator_collapse((void**)(&(o->data)),o->length*sizeof(sll_object_t*));
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_new_object_list(const sll_char_t* t,sll_string_length_t tl,sll_var_arg_list_t* va){
	if (!tl){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	SKIP_MODIFIERS;
	sll_object_t* e=_build_single(&t,&tl,va);
	if (!tl){
		return e;
	}
	sll_object_t* o=sll_array_length_to_object(1);
	o->dt.array.data[0]=e;
	do{
		o->dt.array.length++;
		sll_allocator_resize((void**)(&(o->dt.array.data)),o->dt.array.length*sizeof(sll_object_t*));
		o->dt.array.data[o->dt.array.length-1]=_build_single(&t,&tl,va);
	} while (tl);
	sll_allocator_collapse((void**)(&(o->dt.array.data)),o->dt.array.length*sizeof(sll_object_t*));
	return o;
}

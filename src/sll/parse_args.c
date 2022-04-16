#include <sll/_internal/common.h>
#include <sll/_internal/gc.h>
#include <sll/_internal/parse_args.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/complex.h>
#include <sll/gc.h>
#include <sll/map.h>
#include <sll/memory.h>
#include <sll/object.h>
#include <sll/operator.h>
#include <sll/parse_args.h>
#include <sll/static_object.h>
#include <sll/types.h>
#include <stdarg.h>



#define SKIP_WHITESPACE \
	while (*t==' '||(*t>8&&*t<14)||*t=='!'){ \
		t++; \
	}

#define INIT_ZERO(x) \
	do{ \
		*(x)=0; \
	} while (0)

#define GET_PTR(type) (o->t==ARG_OUTPUT_TYPE_C?va_arg(*(o->dt.c),type*):(type*)_get_ptr_array(o,sizeof(type)))

#define ENSURE_TYPE(var,name) \
	if (var->t!=SLL_OBJECT_TYPE_##name){ \
		var=sll_operator_cast(var,sll_static_int[SLL_OBJECT_TYPE_##name]); \
		if (!*st){ \
			*st=sll_allocate_stack(sizeof(arg_state_t)+sizeof(sll_object_t*)); \
			(*st)->sz++; \
		} \
		else{ \
			(*st)->sz++; \
			*st=sll_reallocate(*st,sizeof(arg_state_t)+(*st)->sz*sizeof(sll_object_t*)); \
		} \
		(*st)->dt[(*st)->sz-1]=var; \
	} \

#define PARSE_TYPE(type,name,field,init) \
	if (arr){ \
		SLL_UNIMPLEMENTED(); \
	} \
	type* var=GET_PTR(type); \
	if (!arg){ \
		init(var); \
		return; \
	} \
	sll_object_t* obj=sll_operator_cast(arg,sll_static_int[SLL_OBJECT_TYPE_##name]); \
	*var=obj->dt.field; \
	GC_RELEASE(obj);
#define PARSE_TYPE_RANGE(type,base_type,name,field) \
	if (arr){ \
		SLL_UNIMPLEMENTED(); \
	} \
	SLL_ASSERT(o->t==ARG_OUTPUT_TYPE_C); \
	type* var=va_arg(*(o->dt.c),type*); \
	type mn=(type)va_arg(*(o->dt.c),base_type); \
	type mx=(type)va_arg(*(o->dt.c),base_type); \
	if (mx<mn){ \
		type tmp=mn; \
		mn=mx; \
		mx=tmp; \
	} \
	if (!arg){ \
		*var=mn; \
		return; \
	} \
	sll_object_t* obj=sll_operator_cast(arg,sll_static_int[SLL_OBJECT_TYPE_##name]); \
	*var=obj->dt.field; \
	GC_RELEASE(obj);\
	if (*var<mn){ \
		*var=mn; \
	} \
	else if (*var>mx){ \
		*var=mx; \
	}
#define PARSE_TYPE_PTR(type,name,field,init) \
	if (arr){ \
		sll_object_t* obj=sll_operator_cast(arg,sll_static_int[SLL_OBJECT_TYPE_ARRAY]); \
		type** dt=sll_allocate(obj->dt.a.l*sizeof(type*)); \
		*GET_PTR(type**)=dt; \
		*GET_PTR(sll_arg_count_t)=obj->dt.a.l; \
		for (sll_arg_count_t i=0;i<obj->dt.a.l;i++){ \
			sll_object_t* k=obj->dt.a.v[i]; \
			ENSURE_TYPE(k,name); \
			*(dt+i)=&(k->dt.field); \
		} \
		GC_RELEASE(obj); \
		return; \
	} \
	type** var=GET_PTR(type*); \
	if (!arg){ \
		*var=NULL; \
		return; \
	} \
	ENSURE_TYPE(arg,name); \
	*var=&(arg->dt.field);
#define PARSE_TYPES(type,name1,field1,name2,field2,init) \
	if (arr){ \
		SLL_UNIMPLEMENTED(); \
	} \
	type* var=GET_PTR(type); \
	if (!arg){ \
		var->t=SLL_PARSE_ARGS_TYPE_##name2; \
		init(&(var->dt.field2)); \
	} \
	else if (arg->t==SLL_OBJECT_TYPE_##name1){ \
		var->t=SLL_PARSE_ARGS_TYPE_##name1; \
		var->dt.field1=arg->dt.field1; \
	} \
	else if (arg->t==SLL_OBJECT_TYPE_##name2){ \
		var->t=SLL_PARSE_ARGS_TYPE_##name2; \
		var->dt.field2=arg->dt.field2; \
	} \
	else{ \
		SLL_UNIMPLEMENTED(); \
	}



static __SLL_FORCE_INLINE void* _get_ptr_array(arg_output_t* o,sll_size_t sz){
	SLL_ASSERT(o->t==ARG_OUTPUT_TYPE_ARRAY);
	sz=(sz+7)&0xfffffffffffffff8ull;
	SLL_ASSERT(o->dt.arr.sz>=sz);
	void* ptr=o->dt.arr.ptr;
	o->dt.arr.ptr=PTR(ADDR(o->dt.arr.ptr)+sz);
	o->dt.arr.sz-=sz;
	return ptr;
}



static void _parse_bool(sll_object_t* arg,sll_bool_t arr,arg_state_t** st,arg_output_t* o){
	if (arr){
		SLL_UNIMPLEMENTED();
	}
	*GET_PTR(sll_bool_t)=(arg?sll_operator_bool(arg):0);
}



static void _parse_int(sll_object_t* arg,sll_bool_t arr,arg_state_t** st,arg_output_t* o){
	PARSE_TYPE(sll_integer_t,INT,i,INIT_ZERO);
}



static void _parse_int_range(sll_object_t* arg,sll_bool_t arr,arg_state_t** st,arg_output_t* o){
	PARSE_TYPE_RANGE(sll_integer_t,sll_integer_t,INT,i);
}



static void _parse_float(sll_object_t* arg,sll_bool_t arr,arg_state_t** st,arg_output_t* o){
	PARSE_TYPE(sll_float_t,FLOAT,f,INIT_ZERO);
}



static void _parse_float_range(sll_object_t* arg,sll_bool_t arr,arg_state_t** st,arg_output_t* o){
	PARSE_TYPE_RANGE(sll_float_t,sll_float_t,FLOAT,f);
}



static void _parse_char(sll_object_t* arg,sll_bool_t arr,arg_state_t** st,arg_output_t* o){
	PARSE_TYPE(sll_char_t,CHAR,c,INIT_ZERO);
}



static void _parse_char_range(sll_object_t* arg,sll_bool_t arr,arg_state_t** st,arg_output_t* o){
	PARSE_TYPE_RANGE(sll_char_t,__SLL_U32,CHAR,c);
}



static void _parse_complex(sll_object_t* arg,sll_bool_t arr,arg_state_t** st,arg_output_t* o){
	PARSE_TYPE(sll_complex_t,COMPLEX,d,SLL_INIT_COMPLEX);
}



static void _parse_string(sll_object_t* arg,sll_bool_t arr,arg_state_t** st,arg_output_t* o){
	PARSE_TYPE_PTR(sll_string_t,STRING,s,SLL_INIT_STRING);
}



static void _parse_char_or_string(sll_object_t* arg,sll_bool_t arr,arg_state_t** st,arg_output_t* o){
	PARSE_TYPES(sll_char_string_t,CHAR,c,STRING,s,SLL_INIT_STRING);
}



static void _parse_array(sll_object_t* arg,sll_bool_t arr,arg_state_t** st,arg_output_t* o){
	PARSE_TYPE_PTR(sll_array_t,ARRAY,a,SLL_INIT_ARRAY);
}



static void _parse_map(sll_object_t* arg,sll_bool_t arr,arg_state_t** st,arg_output_t* o){
	PARSE_TYPE_PTR(sll_map_t,MAP,m,SLL_INIT_MAP);
}



static void _parse_int_or_float(sll_object_t* arg,sll_bool_t arr,arg_state_t** st,arg_output_t* o){
	PARSE_TYPES(sll_int_float_t,INT,i,FLOAT,f,INIT_ZERO);
}



static void _parse_object(sll_object_t* arg,sll_bool_t arr,arg_state_t** st,arg_output_t* o){
	if (arr){
		SLL_UNIMPLEMENTED();
	}
	*GET_PTR(sll_object_t*)=(arg?arg:SLL_ACQUIRE_STATIC_INT(0));
}



sll_arg_state_t _parse_args_raw(const sll_char_t* t,sll_object_t*const* al,sll_arg_count_t all,arg_output_t* o){
	SKIP_WHITESPACE;
	if (!(*t)){
		return NULL;
	}
	arg_state_t* st=NULL;
	const sll_char_t* tmp=t;
	sll_bool_t var_arg=0;
	sll_string_length_t var_arg_idx=0;
	while (*tmp){
		if (*tmp=='!'){
			var_arg=1;
		}
		else if (*tmp=='b'||*tmp=='i'||*tmp=='I'||*tmp=='f'||*tmp=='F'||*tmp=='x'||*tmp=='c'||*tmp=='C'||*tmp=='d'||*tmp=='s'||*tmp=='y'||*tmp=='a'||*tmp=='m'||*tmp=='o'){
			var_arg_idx++;
		}
		tmp++;
	}
	if (!var_arg){
		var_arg_idx=SLL_MAX_STRING_INDEX;
	}
	while (*t){
		sll_char_t type=*t;
		t++;
		sll_bool_t arr=0;
		while (1){
			SKIP_WHITESPACE;
			if (*t=='+'){
				arr=1;
			}
			else{
				break;
			}
			t++;
		}
		var_arg_idx--;
		if (!var_arg_idx){
			if (arr){
				SLL_UNIMPLEMENTED();
			}
			switch (type){
				case 'b':
					SLL_UNIMPLEMENTED();
				case 'i':
					SLL_UNIMPLEMENTED();
				case 'I':
					SLL_UNIMPLEMENTED();
				case 'f':
					SLL_UNIMPLEMENTED();
				case 'F':
					SLL_UNIMPLEMENTED();
				case 'x':
					SLL_UNIMPLEMENTED();
				case 'c':
					SLL_UNIMPLEMENTED();
				case 'C':
					SLL_UNIMPLEMENTED();
				case 'd':
					SLL_UNIMPLEMENTED();
				case 's':
					SLL_UNIMPLEMENTED();
				case 'y':
					SLL_UNIMPLEMENTED();
				case 'a':
					SLL_UNIMPLEMENTED();
				case 'm':
					SLL_UNIMPLEMENTED();
				case 'o':
					{
						sll_object_t** dt=sll_allocate(all*sizeof(sll_object_t*));
						*GET_PTR(sll_object_t**)=dt;
						*GET_PTR(sll_arg_count_t)=all;
						while (all){
							all--;
							*(dt+all)=*(al+all);
						}
						break;
					}
				default:
					SLL_UNREACHABLE();
			}
			break;
		}
		sll_object_t* arg=NULL;
		if (all){
			arg=*al;
			al++;
			all--;
		}
		void (*fn)(sll_object_t*,sll_bool_t,arg_state_t**,arg_output_t*)=NULL;
		switch (type){
			case 'b':
				fn=_parse_bool;
				break;
			case 'i':
				fn=_parse_int;
				break;
			case 'I':
				fn=_parse_int_range;
				break;
			case 'f':
				fn=_parse_float;
				break;
			case 'F':
				fn=_parse_float_range;
				break;
			case 'x':
				fn=_parse_int_or_float;
				break;
			case 'c':
				fn=_parse_char;
				break;
			case 'C':
				fn=_parse_char_range;
				break;
			case 'd':
				fn=_parse_complex;
				break;
			case 's':
				fn=_parse_string;
				break;
			case 'y':
				fn=_parse_char_or_string;
				break;
			case 'a':
				fn=_parse_array;
				break;
			case 'm':
				fn=_parse_map;
				break;
			case 'o':
				fn=_parse_object;
				break;
		}
		if (fn){
			fn(arg,arr,&st,o);
		}
	}
	if (st){
		return sll_memory_move(st,SLL_MEMORY_MOVE_DIRECTION_FROM_STACK);
	}
	sll_deallocate(st);
	return NULL;
}



__SLL_EXTERNAL void sll_free_args(sll_arg_state_t dt){
	if (!dt){
		return;
	}
	arg_state_t* st=dt;
	for (sll_size_t i=0;i<st->sz;i++){
		GC_RELEASE(st->dt[i]);
	}
	sll_deallocate(dt);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_size_t sll_parse_arg_count(const sll_char_t* t,sll_arg_count_t* o){
	SKIP_WHITESPACE;
	sll_arg_count_t ac=0;
	sll_size_t sz=0;
	sll_bool_t va=0;
	sll_bool_t arr=0;
	while (*t){
		if (*t=='!'){
			va=1;
		}
		else if (*t=='+'){
			if (!arr){
				if (ac){
					sz+=8;
					ac++;
				}
				arr=1;
			}
		}
		else if (*t=='I'||*t=='F'||*t=='C'){
			SLL_UNIMPLEMENTED();
		}
		else if (*t=='b'||*t=='i'||*t=='f'||*t=='x'||*t=='c'||*t=='d'||*t=='s'||*t=='y'||*t=='a'||*t=='m'||*t=='o'){
			ac++;
			sz+=(*t=='x'||*t=='d'||*t=='y'?16:8);
			arr=0;
		}
		t++;
		SKIP_WHITESPACE;
	}
	if (ac){
		ac+=va;
		sz+=8;
	}
	*o=ac;
	return sz;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_arg_state_t sll_parse_args(const sll_char_t* t,sll_object_t*const* al,sll_arg_count_t all,...){
	SKIP_WHITESPACE;
	if (!(*t)){
		return NULL;
	}
	va_list va;
	va_start(va,all);
	sll_arg_state_t o=sll_parse_args_list(t,al,all,&va);
	va_end(va);
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_arg_state_t sll_parse_args_list(const sll_char_t* t,sll_object_t*const* al,sll_arg_count_t all,va_list* va){
	arg_output_t o={
		ARG_OUTPUT_TYPE_C,
		{
			.c=va
		}
	};
	return _parse_args_raw(t,al,all,&o);
}

#include <sll/_internal/common.h>
#include <sll/_internal/gc.h>
#include <sll/_internal/parse_args.h>
#include <sll/_size_types.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/complex.h>
#include <sll/gc.h>
#include <sll/ift.h>
#include <sll/map.h>
#include <sll/memory.h>
#include <sll/object.h>
#include <sll/operator.h>
#include <sll/parse_args.h>
#include <sll/static_object.h>
#include <sll/types.h>
#include <stdarg.h>



#define SKIP_WHITESPACE \
	while (*t==' '||(*t>8&&*t<14)||*t=='!'||*t=='#'){ \
		t++; \
	}
#define SKIP_WHITESPACE_KEEP_VA \
	while (*t==' '||(*t>8&&*t<14)||*t=='#'){ \
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
#define PARSE_INT(sz) \
	if (arr){ \
		SLL_UNIMPLEMENTED(); \
	} \
	__SLL_U##sz* var=GET_PTR(__SLL_U##sz); \
	if (!arg){ \
		*var=0; \
		return; \
	} \
	sll_object_t* obj=sll_operator_cast(arg,sll_static_int[SLL_OBJECT_TYPE_INT]); \
	*var=(obj->dt.i<0?0:(obj->dt.i>__SLL_U##sz##_MAX?__SLL_U##sz##_MAX:(__SLL_U##sz)(obj->dt.i))); \
	GC_RELEASE(obj);
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

#define PUSH_REGISTER(wide) \
	do{ \
		if (regs){ \
			if (!(ac&63)){ \
				reg_sz++; \
				*regs=sll_reallocate(*regs,reg_sz*sizeof(bitmap_t)); \
				*(*regs+reg_sz-1)=0; \
			} \
			*(*regs+reg_sz-1)|=(wide)<<(ac&63); \
		} \
	} while (0)



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



static void _parse_int8(sll_object_t* arg,sll_bool_t arr,arg_state_t** st,arg_output_t* o){
	PARSE_INT(8);
}



static void _parse_int16(sll_object_t* arg,sll_bool_t arr,arg_state_t** st,arg_output_t* o){
	PARSE_INT(16);
}



static void _parse_int32(sll_object_t* arg,sll_bool_t arr,arg_state_t** st,arg_output_t* o){
	PARSE_INT(32);
}



static void _parse_int(sll_object_t* arg,sll_bool_t arr,arg_state_t** st,arg_output_t* o){
	PARSE_TYPE(sll_integer_t,INT,i,INIT_ZERO);
}



static void _parse_float(sll_object_t* arg,sll_bool_t arr,arg_state_t** st,arg_output_t* o){
	PARSE_TYPE(sll_float_t,FLOAT,f,INIT_ZERO);
}



static void _parse_int_or_float(sll_object_t* arg,sll_bool_t arr,arg_state_t** st,arg_output_t* o){
	if (arr){
		SLL_UNIMPLEMENTED();
	}
	sll_int_float_t* var=GET_PTR(sll_int_float_t);
	if (!arg){
		var->t=SLL_PARSE_ARGS_TYPE_FLOAT;
		var->dt.f=0;
	}
	else if (arg->t==SLL_OBJECT_TYPE_INT){
		var->t=SLL_PARSE_ARGS_TYPE_INT;
		var->dt.i=arg->dt.i;
	}
	else if (arg->t==SLL_OBJECT_TYPE_FLOAT){
		var->t=SLL_PARSE_ARGS_TYPE_FLOAT;
		var->dt.f=arg->dt.f;
	}
	else{
		SLL_UNIMPLEMENTED();
	}
}



static void _parse_char(sll_object_t* arg,sll_bool_t arr,arg_state_t** st,arg_output_t* o){
	PARSE_TYPE(sll_char_t,CHAR,c,INIT_ZERO);
}



static void _parse_complex(sll_object_t* arg,sll_bool_t arr,arg_state_t** st,arg_output_t* o){
	PARSE_TYPE(sll_complex_t,COMPLEX,d,SLL_INIT_COMPLEX);
}



static void _parse_string(sll_object_t* arg,sll_bool_t arr,arg_state_t** st,arg_output_t* o){
	PARSE_TYPE_PTR(sll_string_t,STRING,s,SLL_INIT_STRING);
}



static void _parse_char_or_string(sll_object_t* arg,sll_bool_t arr,arg_state_t** st,arg_output_t* o){
	if (arr){
		SLL_UNIMPLEMENTED();
	}
	sll_char_string_t* var=GET_PTR(sll_char_string_t);
	if (!arg){
		SLL_UNIMPLEMENTED();
	}
	else if (arg->t==SLL_OBJECT_TYPE_CHAR){
		var->t=SLL_PARSE_ARGS_TYPE_CHAR;
		var->dt.c=arg->dt.c;
	}
	else if (arg->t==SLL_OBJECT_TYPE_STRING){
		var->t=SLL_PARSE_ARGS_TYPE_STRING;
		var->dt.s=&(arg->dt.s);
	}
	else{
		SLL_UNIMPLEMENTED();
	}
}



static void _parse_array(sll_object_t* arg,sll_bool_t arr,arg_state_t** st,arg_output_t* o){
	PARSE_TYPE_PTR(sll_array_t,ARRAY,a,SLL_INIT_ARRAY);
}



static void _parse_map(sll_object_t* arg,sll_bool_t arr,arg_state_t** st,arg_output_t* o){
	PARSE_TYPE_PTR(sll_map_t,MAP,m,SLL_INIT_MAP);
}



static void _parse_object(sll_object_t* arg,sll_bool_t arr,arg_state_t** st,arg_output_t* o){
	if (arr){
		SLL_UNIMPLEMENTED();
	}
	*GET_PTR(sll_object_t*)=(arg?arg:SLL_ACQUIRE_STATIC_INT(0));
}



sll_arg_count_t _parse_arg_count(const sll_char_t* t,sll_char_t ret,bitmap_t** regs,sll_size_t* o){
	SKIP_WHITESPACE_KEEP_VA;
	sll_size_t reg_sz=0;
	if (regs){
		*regs=NULL;
	}
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
					PUSH_REGISTER(0);
					ac++;
				}
				arr=1;
			}
		}
		else if (*t=='b'||*t=='B'||*t=='W'||*t=='D'||*t=='Q'||*t=='i'||*t=='f'||*t=='x'||*t=='c'||*t=='d'||*t=='s'||*t=='y'||*t=='a'||*t=='m'||*t=='o'){
			sz+=8;
			arr=0;
			sll_bool_t wide=0;
			if (*t=='x'||*t=='d'||*t=='y'){
				sz+=8;
				wide=1;
			}
			PUSH_REGISTER(wide);
			ac++;
		}
		t++;
		SKIP_WHITESPACE_KEEP_VA;
	}
	if (ac&&va){
		sz+=8;
		PUSH_REGISTER(0);
		ac++;
	}
	PUSH_REGISTER((ret=='d'||ret=='s'||ret=='a'||ret=='m'));
	SLL_ASSERT(!regs||reg_sz==(ac+64)>>6);
	if (o){
		*o=sz;
	}
	return ac;
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
		else if (*tmp=='b'||*tmp=='B'||*tmp=='W'||*tmp=='D'||*tmp=='Q'||*tmp=='i'||*tmp=='f'||*tmp=='x'||*tmp=='c'||*tmp=='d'||*tmp=='s'||*tmp=='y'||*tmp=='a'||*tmp=='m'||*tmp=='o'){
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
				case 'B':
					SLL_UNIMPLEMENTED();
				case 'W':
					SLL_UNIMPLEMENTED();
				case 'D':
					SLL_UNIMPLEMENTED();
				case 'Q':
				case 'i':
					SLL_UNIMPLEMENTED();
				case 'f':
					SLL_UNIMPLEMENTED();
				case 'x':
					SLL_UNIMPLEMENTED();
				case 'c':
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
			case 'B':
				fn=_parse_int8;
				break;
			case 'W':
				fn=_parse_int16;
				break;
			case 'D':
				fn=_parse_int32;
				break;
			case 'Q':
			case 'i':
				fn=_parse_int;
				break;
			case 'f':
				fn=_parse_float;
				break;
			case 'x':
				fn=_parse_int_or_float;
				break;
			case 'c':
				fn=_parse_char;
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



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_arg_count_t sll_parse_arg_count(const sll_char_t* t){
	return _parse_arg_count(t,0,NULL,NULL);
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

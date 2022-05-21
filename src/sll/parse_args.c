#include <sll/_internal/api_call.h>
#include <sll/_internal/common.h>
#include <sll/_internal/parse_args.h>
#include <sll/_size_types.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/complex.h>
#include <sll/gc.h>
#include <sll/ift.h>
#include <sll/log.h>
#include <sll/map.h>
#include <sll/memory.h>
#include <sll/object.h>
#include <sll/operator.h>
#include <sll/parse_args.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/string_table.h>
#include <sll/types.h>
#include <stdarg.h>



#define SKIP_WHITESPACE \
	while (*t&&*t!='b'&&*t!='B'&&*t!='W'&&*t!='D'&&*t!='Q'&&*t!='i'&&*t!='f'&&*t!='x'&&*t!='c'&&*t!='d'&&*t!='X'&&*t!='z'&&*t!='s'&&*t!='y'&&*t!='a'&&*t!='m'&&*t!='o'&&*t!='!'&&*t!='+'&&*t!='&'&&*t!='#'&&*t!='@'){ \
		t++; \
	}

#define INIT_ZERO(x) \
	do{ \
		*(x)=0; \
	} while (0)

#define GET_PTR(type) (o->t==ARG_OUTPUT_TYPE_C?va_arg(*(o->dt.c),type*):(type*)_get_ptr_array(o,sizeof(type)))

#define ENSURE_TYPE(var,name) \
	if (var->type!=SLL_OBJECT_TYPE_##name){ \
		var=sll_operator_cast(var,sll_static_int[SLL_OBJECT_TYPE_##name]); \
		if (!*st){ \
			*st=sll_allocate_stack(sizeof(arg_state_t)+sizeof(sll_object_t*)); \
			(*st)->sz=1; \
		} \
		else{ \
			(*st)->sz++; \
			*st=sll_reallocate(*st,sizeof(arg_state_t)+(*st)->sz*sizeof(sll_object_t*)); \
		} \
		(*st)->dt[(*st)->sz-1]=var; \
	} \

#define PARSE_INT(sz) \
	if (flags&PARSE_ARGS_FLAG_ARRAY){ \
		if (flags&PARSE_ARGS_FLAG_REF){ \
			SLL_UNIMPLEMENTED(); \
		} \
		SLL_UNIMPLEMENTED(); \
	} \
	__SLL_U##sz* var=GET_PTR(__SLL_U##sz); \
	if (!arg){ \
		*var=0; \
		return; \
	} \
	sll_object_t* obj=sll_operator_cast(arg,sll_static_int[SLL_OBJECT_TYPE_INT]); \
	*var=(obj->data.int_<0?0:(obj->data.int_>__SLL_U##sz##_MAX?__SLL_U##sz##_MAX:(__SLL_U##sz)(obj->data.int_))); \
	SLL_RELEASE(obj);
#define PARSE_TYPE(type,name,field,init) \
	if (flags&PARSE_ARGS_FLAG_ARRAY){ \
		if (flags&PARSE_ARGS_FLAG_REF){ \
			SLL_UNIMPLEMENTED(); \
		} \
		SLL_UNIMPLEMENTED(); \
	} \
	type* var=GET_PTR(type); \
	if (!arg){ \
		init(var); \
		return; \
	} \
	sll_object_t* obj=sll_operator_cast(arg,sll_static_int[SLL_OBJECT_TYPE_##name]); \
	*var=obj->data.field; \
	SLL_RELEASE(obj);
#define PARSE_TYPE_PTR(type_,name,field,init) \
	if (flags&PARSE_ARGS_FLAG_ARRAY){ \
		if (flags&PARSE_ARGS_FLAG_REF){ \
			SLL_UNIMPLEMENTED(); \
		} \
		sll_object_t* obj=sll_operator_cast(arg,sll_static_int[SLL_OBJECT_TYPE_ARRAY]); \
		type_** dt=sll_allocate(obj->data.array.length*sizeof(type_*)); \
		*GET_PTR(type_**)=dt; \
		*GET_PTR(sll_arg_count_t)=obj->data.array.length; \
		for (sll_arg_count_t i=0;i<obj->data.array.length;i++){ \
			sll_object_t* k=obj->data.array.data[i]; \
			ENSURE_TYPE(k,name); \
			*(dt+i)=&(k->data.field); \
		} \
		SLL_RELEASE(obj); \
		return; \
	} \
	if (flags&PARSE_ARGS_FLAG_REF){ \
		sll_object_t** var=GET_PTR(sll_object_t*); \
		if (!arg){ \
			*var=SLL_ACQUIRE_STATIC_INT(0); \
		} \
		else{ \
			ENSURE_TYPE(arg,name); \
			*var=arg; \
		} \
		return; \
	} \
	type_** var=GET_PTR(type_*); \
	if ((flags&PARSE_ARGS_FLAG_NULL)&&(!arg||(arg->type==SLL_OBJECT_TYPE_INT&&!arg->data.int_))){ \
		*var=NULL; \
	} \
	else if (!arg){ \
		init(*var); \
	} \
	else{ \
		ENSURE_TYPE(arg,name); \
		*var=&(arg->data.field); \
	}

#define PUSH_REGISTER(a,b) \
	do{ \
		if (regs){ \
			if (reg_bit_count==64){ \
				reg_sz++; \
				*regs=sll_reallocate(*regs,reg_sz*sizeof(bitmap_t)); \
				*(*regs+reg_sz-1)=0; \
				reg_bit_count=0; \
			} \
			*(*regs+reg_sz-1)|=(((!!(b))<<1)|(!!(a)))<<reg_bit_count; \
			reg_bit_count+=2; \
		} \
	} while (0)

#define WARN_IGNORED_CONST(nm) \
	do{ \
		if (flags&PARSE_ARGS_FLAG_CONST){ \
			SLL_WARN("Ignoring 'const' modifier on type "nm": '%s'",tmp); \
		} \
	} while (0)
#define WARN_IGNORED_NULL(nm) \
	do{ \
		if (flags&PARSE_ARGS_FLAG_NULL){ \
			SLL_WARN("Ignoring 'null' modifier on type "nm": '%s'",tmp); \
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



static void _parse_bool(sll_object_t* arg,arg_parse_flags_t flags,arg_state_t** st,arg_output_t* o){
	if (flags&PARSE_ARGS_FLAG_ARRAY){
		if (flags&PARSE_ARGS_FLAG_REF){
			SLL_UNIMPLEMENTED();
		}
		SLL_UNIMPLEMENTED();
	}
	*GET_PTR(sll_bool_t)=(arg?sll_operator_bool(arg):0);
}



static void _parse_uint8(sll_object_t* arg,arg_parse_flags_t flags,arg_state_t** st,arg_output_t* o){
	PARSE_INT(8);
}



static void _parse_uint16(sll_object_t* arg,arg_parse_flags_t flags,arg_state_t** st,arg_output_t* o){
	PARSE_INT(16);
}



static void _parse_uint32(sll_object_t* arg,arg_parse_flags_t flags,arg_state_t** st,arg_output_t* o){
	PARSE_INT(32);
}



static void _parse_int(sll_object_t* arg,arg_parse_flags_t flags,arg_state_t** st,arg_output_t* o){
	PARSE_TYPE(sll_integer_t,INT,int_,INIT_ZERO);
}



static void _parse_float(sll_object_t* arg,arg_parse_flags_t flags,arg_state_t** st,arg_output_t* o){
	PARSE_TYPE(sll_float_t,FLOAT,float_,INIT_ZERO);
}



static void _parse_int_or_float(sll_object_t* arg,arg_parse_flags_t flags,arg_state_t** st,arg_output_t* o){
	if (flags&PARSE_ARGS_FLAG_ARRAY){
		if (flags&PARSE_ARGS_FLAG_REF){
			SLL_UNIMPLEMENTED();
		}
		SLL_UNIMPLEMENTED();
	}
	sll_number_t* var=GET_PTR(sll_number_t);
	if (!arg){
		var->t=SLL_PARSE_ARGS_TYPE_FLOAT;
		var->dt.f=0;
	}
	else if (arg->type==SLL_OBJECT_TYPE_INT){
		var->t=SLL_PARSE_ARGS_TYPE_INT;
		var->dt.i=arg->data.int_;
	}
	else if (arg->type==SLL_OBJECT_TYPE_FLOAT){
		var->t=SLL_PARSE_ARGS_TYPE_FLOAT;
		var->dt.f=arg->data.float_;
	}
	else{
		SLL_UNIMPLEMENTED();
	}
}



static void _parse_char(sll_object_t* arg,arg_parse_flags_t flags,arg_state_t** st,arg_output_t* o){
	PARSE_TYPE(sll_char_t,CHAR,char_,INIT_ZERO);
}



static void _parse_complex(sll_object_t* arg,arg_parse_flags_t flags,arg_state_t** st,arg_output_t* o){
	PARSE_TYPE(sll_complex_t,COMPLEX,complex_,SLL_INIT_COMPLEX);
}



static void _parse_float_or_complex(sll_object_t* arg,arg_parse_flags_t flags,arg_state_t** st,arg_output_t* o){
	if (flags&PARSE_ARGS_FLAG_ARRAY){
		if (flags&PARSE_ARGS_FLAG_REF){
			SLL_UNIMPLEMENTED();
		}
		SLL_UNIMPLEMENTED();
	}
	sll_number_t* var=GET_PTR(sll_number_t);
	var->t=SLL_PARSE_ARGS_TYPE_FLOAT;
	if (!arg){
		var->dt.f=0;
	}
	else if (arg->type==SLL_OBJECT_TYPE_FLOAT){
		var->dt.f=arg->data.float_;
	}
	else if (arg->type==SLL_OBJECT_TYPE_COMPLEX){
		var->t=SLL_PARSE_ARGS_TYPE_COMPLEX;
		var->dt.d=arg->data.complex_;
	}
	else{
		sll_object_t* obj=sll_operator_cast(arg,sll_static_int[SLL_OBJECT_TYPE_FLOAT]);
		var->dt.f=obj->data.float_;
		SLL_RELEASE(obj);
	}
}



static void _parse_int_or_float_or_complex(sll_object_t* arg,arg_parse_flags_t flags,arg_state_t** st,arg_output_t* o){
	if (flags&PARSE_ARGS_FLAG_ARRAY){
		if (flags&PARSE_ARGS_FLAG_REF){
			SLL_UNIMPLEMENTED();
		}
		SLL_UNIMPLEMENTED();
	}
	sll_number_t* var=GET_PTR(sll_number_t);
	var->t=SLL_PARSE_ARGS_TYPE_FLOAT;
	if (!arg){
		var->dt.f=0;
	}
	else if (arg->type==SLL_OBJECT_TYPE_INT){
		var->t=SLL_PARSE_ARGS_TYPE_INT;
		var->dt.i=arg->data.int_;
	}
	else if (arg->type==SLL_OBJECT_TYPE_FLOAT){
		var->dt.f=arg->data.float_;
	}
	else if (arg->type==SLL_OBJECT_TYPE_COMPLEX){
		var->t=SLL_PARSE_ARGS_TYPE_COMPLEX;
		var->dt.d=arg->data.complex_;
	}
	else{
		sll_object_t* obj=sll_operator_cast(arg,sll_static_int[SLL_OBJECT_TYPE_FLOAT]);
		var->dt.f=obj->data.float_;
		SLL_RELEASE(obj);
	}
}



static void _parse_string(sll_object_t* arg,arg_parse_flags_t flags,arg_state_t** st,arg_output_t* o){
	PARSE_TYPE_PTR(sll_string_t,STRING,string,SLL_INIT_STRING);
}



static void _parse_char_or_string(sll_object_t* arg,arg_parse_flags_t flags,arg_state_t** st,arg_output_t* o){
	if (flags&PARSE_ARGS_FLAG_ARRAY){
		if (flags&PARSE_ARGS_FLAG_REF){
			SLL_UNIMPLEMENTED();
		}
		SLL_UNIMPLEMENTED();
	}
	if (flags&PARSE_ARGS_FLAG_REF){
		SLL_UNIMPLEMENTED();
	}
	sll_char_string_t* var=GET_PTR(sll_char_string_t);
	if (!arg){
		SLL_UNIMPLEMENTED();
	}
	else if (arg->type==SLL_OBJECT_TYPE_CHAR){
		var->t=SLL_PARSE_ARGS_TYPE_CHAR;
		var->dt.c=arg->data.char_;
	}
	else if (arg->type==SLL_OBJECT_TYPE_STRING){
		var->t=SLL_PARSE_ARGS_TYPE_STRING;
		var->dt.s=&(arg->data.string);
	}
	else{
		SLL_UNIMPLEMENTED();
	}
}



static void _parse_array(sll_object_t* arg,arg_parse_flags_t flags,arg_state_t** st,arg_output_t* o){
	PARSE_TYPE_PTR(sll_array_t,ARRAY,array,SLL_INIT_ARRAY);
}



static void _parse_map(sll_object_t* arg,arg_parse_flags_t flags,arg_state_t** st,arg_output_t* o){
	PARSE_TYPE_PTR(sll_map_t,MAP,map,SLL_INIT_MAP);
}



static void _parse_object(sll_object_t* arg,arg_parse_flags_t flags,arg_state_t** st,arg_output_t* o){
	if (flags&PARSE_ARGS_FLAG_ARRAY){
		if (flags&PARSE_ARGS_FLAG_REF){
			ENSURE_TYPE(arg,ARRAY);
			*GET_PTR(sll_object_t*)=arg;
			return;
		}
		SLL_UNIMPLEMENTED();
	}
	*GET_PTR(sll_object_t*)=(arg?arg:SLL_ACQUIRE_STATIC_INT(0));
}



sll_arg_count_t _parse_arg_count(const sll_char_t* t,__SLL_U16 ret,bitmap_t** regs,sll_size_t* o){
	SKIP_WHITESPACE;
	sll_size_t reg_sz=0;
	__SLL_U8 reg_bit_count=64;
	if (regs){
		*regs=NULL;
	}
	sll_arg_count_t ac=0;
	sll_size_t sz=0;
	sll_bool_t va=0;
	sll_bool_t arr=0;
	sll_bool_t ref=0;
	while (*t){
		if (*t=='!'){
			va=1;
		}
		else if (*t=='+'){
			if (!arr){
				if (ac){
					sz+=8;
					PUSH_REGISTER(0,0);
					ac++;
				}
				arr=1;
			}
		}
		else if (*t=='&'){
			ref=1;
		}
		else if (*t=='b'||*t=='B'||*t=='W'||*t=='D'||*t=='Q'||*t=='i'||*t=='f'||*t=='x'||*t=='c'||*t=='d'||*t=='X'||*t=='z'||*t=='s'||*t=='y'||*t=='a'||*t=='m'||*t=='o'){
			sz+=8;
			arr=0;
			sll_bool_t wide=0;
			sll_bool_t extra_wide=0;
			if (!ref&&(*t=='d'||*t=='y')){
				sz+=8;
				wide=1;
			}
			else if (!ref&&(*t=='x'||*t=='X'||*t=='z')){
				sz+=16;
				extra_wide=1;
			}
			PUSH_REGISTER(wide,extra_wide);
			ref=0;
			ac++;
		}
		t++;
		SKIP_WHITESPACE;
	}
	if (ac&&va){
		sz+=8;
		PUSH_REGISTER(0,0);
		ac++;
	}
	PUSH_REGISTER(((ret&RETURN_VALUE_FLAG_ERROR)||RETURN_VALUE_GET_TYPE(ret)=='d'||RETURN_VALUE_GET_TYPE(ret)=='x'||RETURN_VALUE_GET_TYPE(ret)=='X'||RETURN_VALUE_GET_TYPE(ret)=='z'||RETURN_VALUE_GET_TYPE(ret)=='s'||RETURN_VALUE_GET_TYPE(ret)=='a'||RETURN_VALUE_GET_TYPE(ret)=='m'),(ret&RETURN_VALUE_FLAG_ERROR));
	SLL_ASSERT(!regs||reg_sz==(ac>>5)+1);
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
	const sll_char_t* tmp=t;
	sll_bool_t var_arg=0;
	sll_string_length_t var_arg_idx=0;
	while (*tmp){
		if (*tmp=='!'){
			var_arg=1;
		}
		else if (*tmp=='b'||*tmp=='B'||*tmp=='W'||*tmp=='D'||*tmp=='Q'||*tmp=='i'||*tmp=='f'||*tmp=='x'||*tmp=='c'||*tmp=='d'||*tmp=='X'||*tmp=='z'||*tmp=='s'||*tmp=='y'||*tmp=='a'||*tmp=='m'||*tmp=='o'){
			var_arg_idx++;
		}
		tmp++;
	}
	if (!var_arg){
		var_arg_idx=SLL_MAX_STRING_INDEX;
	}
	tmp=t;
	arg_state_t* st=NULL;
	arg_parse_flags_t flags=0;
	while (1){
		SKIP_WHITESPACE;
		if (*t=='&'){
			flags|=PARSE_ARGS_FLAG_REF;
		}
		else if (*t=='#'){
			flags|=PARSE_ARGS_FLAG_CONST;
		}
		else if (*t=='@'){
			flags|=PARSE_ARGS_FLAG_NULL;
		}
		else if (*t!='+'&&*t!='!'){
			break;
		}
		t++;
	}
	while (*t){
		sll_char_t type=*t;
		t++;
		arg_parse_flags_t n_flags=0;
		while (1){
			SKIP_WHITESPACE;
			if (*t=='+'){
				flags|=PARSE_ARGS_FLAG_ARRAY;
			}
			else if (*t=='&'){
				n_flags|=PARSE_ARGS_FLAG_REF;
			}
			else if (*t=='#'){
				n_flags|=PARSE_ARGS_FLAG_CONST;
			}
			else if (*t=='@'){
				n_flags|=PARSE_ARGS_FLAG_NULL;
			}
			else if (*t!='!'){
				break;
			}
			t++;
		}
		var_arg_idx--;
		if (!var_arg_idx){
			if (flags&(PARSE_ARGS_FLAG_ARRAY|PARSE_ARGS_FLAG_REF)){
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
				case 'X':
					SLL_UNIMPLEMENTED();
				case 'z':
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
		void (*fn)(sll_object_t*,arg_parse_flags_t,arg_state_t**,arg_output_t*)=NULL;
		switch (type){
			case 'b':
				WARN_IGNORED_CONST("'sll_bool_t' (b)");
				WARN_IGNORED_NULL("'sll_bool_t' (b)");
				fn=_parse_bool;
				break;
			case 'B':
				WARN_IGNORED_CONST("'__SLL_U8' (B)");
				WARN_IGNORED_NULL("'__SLL_U8' (B)");
				fn=_parse_uint8;
				break;
			case 'W':
				WARN_IGNORED_CONST("'__SLL_U16' (W)");
				WARN_IGNORED_NULL("'__SLL_U16' (W)");
				fn=_parse_uint16;
				break;
			case 'D':
				WARN_IGNORED_CONST("'__SLL_U32' (D)");
				WARN_IGNORED_NULL("'__SLL_U32' (D)");
				fn=_parse_uint32;
				break;
			case 'Q':
			case 'i':
				WARN_IGNORED_CONST("'sll_integer_t' or '__SLL_U64' (i or Q)");
				WARN_IGNORED_NULL("'sll_integer_t' or '__SLL_U64' (i or Q)");
				fn=_parse_int;
				break;
			case 'f':
				WARN_IGNORED_CONST("'sll_float_t' (f)");
				WARN_IGNORED_NULL("'sll_float_t' (f)");
				fn=_parse_float;
				break;
			case 'x':
				WARN_IGNORED_CONST("'const sll_number_t*' (x)");
				WARN_IGNORED_NULL("'const sll_number_t*' (x)");
				fn=_parse_int_or_float;
				break;
			case 'c':
				WARN_IGNORED_CONST("'sll_char_t' (c)");
				WARN_IGNORED_NULL("'sll_char_t' (c)");
				fn=_parse_char;
				break;
			case 'd':
				WARN_IGNORED_CONST("'const sll_complex_t*' (d)");
				WARN_IGNORED_NULL("'const sll_complex_t*' (d)");
				fn=_parse_complex;
				break;
			case 'X':
				WARN_IGNORED_CONST("'const sll_number_t*' (X)");
				WARN_IGNORED_NULL("'const sll_number_t*' (X)");
				fn=_parse_float_or_complex;
				break;
			case 'z':
				WARN_IGNORED_CONST("'const sll_number_t*' (z)");
				WARN_IGNORED_NULL("'const sll_number_t*' (z)");
				fn=_parse_int_or_float_or_complex;
				break;
			case 's':
				fn=_parse_string;
				break;
			case 'y':
				WARN_IGNORED_CONST("'const sll_char_string_t*' (y)");
				fn=_parse_char_or_string;
				break;
			case 'a':
				fn=_parse_array;
				break;
			case 'm':
				fn=_parse_map;
				break;
			case 'o':
				WARN_IGNORED_NULL("'sll_object_t*' (o)");
				fn=_parse_object;
				break;
		}
		if (fn){
			if ((flags&(PARSE_ARGS_FLAG_NULL))&&(flags&(PARSE_ARGS_FLAG_ARRAY|PARSE_ARGS_FLAG_REF))){
				SLL_UNIMPLEMENTED();
			}
			fn(arg,flags,&st,o);
			flags=n_flags;
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
		SLL_RELEASE(st->dt[i]);
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

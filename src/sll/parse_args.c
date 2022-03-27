#include <sll/_internal/common.h>
#include <sll/array.h>
#include <sll/common.h>
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
	while (*t==' '||(*t>8&&*t<14)||*t=='+'){ \
		t++; \
	}

#define INIT_ZERO(x) \
	do{ \
		*(x)=0; \
	} while (0)

#define PARSE_TYPE(type,field,name) \
	{ \
		type* var=ptr; \
		if (!all){ \
			*var=0; \
			break; \
		} \
		sll_object_t* obj=sll_operator_cast(*al,sll_static_int[SLL_OBJECT_TYPE_##name]); \
		*var=obj->dt.field; \
		sll_release_object(obj);\
		al++; \
		all--; \
		break; \
	}
#define PARSE_TYPE_RANGE(type,base_type,field,name) \
	{ \
		type* var=ptr; \
		type mn=(type)va_arg(va,base_type); \
		type mx=(type)va_arg(va,base_type); \
		if (mx<mn){ \
			type tmp=mn; \
			mn=mx; \
			mx=tmp; \
		} \
		if (!all){ \
			*var=mn; \
			break; \
		} \
		sll_object_t* obj=sll_operator_cast(*al,sll_static_int[SLL_OBJECT_TYPE_##name]); \
		*var=obj->dt.field; \
		sll_release_object(obj);\
		if (*var<mn){ \
			*var=mn; \
		} \
		else if (*var>mx){ \
			*var=mx; \
		} \
		al++; \
		all--; \
		break; \
	}
#define PARSE_TYPE_PTR(type,field,name,init) \
	{ \
		type** var=ptr; \
		if (!all){ \
			*var=NULL; \
			break; \
		} \
		sll_object_t* obj=sll_operator_cast(*al,sll_static_int[SLL_OBJECT_TYPE_##name]); \
		*var=&(obj->dt.field); \
		sll_release_object(obj);\
		al++; \
		all--; \
		break; \
	}
#define PARSE_TYPES(type,name1,field1,name2,field2,init) \
	{ \
		type* var=ptr; \
		if (!all){ \
			var->t=SLL_PARSE_ARGS_TYPE_##name2; \
			init(&(var->dt.field2)); \
			break; \
		} \
		if ((*al)->t==SLL_OBJECT_TYPE_##name1){ \
			var->t=SLL_PARSE_ARGS_TYPE_##name1; \
			var->dt.field1=(*al)->dt.field1; \
		} \
		else if ((*al)->t==SLL_OBJECT_TYPE_##name2){ \
			var->t=SLL_PARSE_ARGS_TYPE_##name2; \
			var->dt.field2=(*al)->dt.field2; \
		} \
		else{ \
			SLL_UNIMPLEMENTED(); \
		} \
		al++; \
		all--; \
		break; \
	}



__SLL_EXTERNAL void sll_parse_args(const sll_char_t* t,sll_object_t*const* al,sll_arg_count_t all,...){
	SKIP_WHITESPACE;
	if (!(*t)){
		return;
	}
	const sll_char_t* tmp=t;
	sll_bool_t var_arg=0;
	sll_string_length_t var_arg_idx=0;
	while (*tmp){
		if (*tmp=='+'){
			var_arg=1;
		}
		else if (*tmp=='b'||*tmp=='i'||*tmp=='f'||*tmp=='x'||*tmp=='c'||*tmp=='s'||*tmp=='y'||*tmp=='a'||*tmp=='m'||*tmp=='o'){
			var_arg_idx++;
		}
		tmp++;
	}
	if (!var_arg){
		var_arg_idx=SLL_MAX_STRING_INDEX;
	}
	va_list va;
	va_start(va,all);
	while (*t){
		void* ptr=va_arg(va,void*);
		sll_char_t st=*t;
		t++;
		SKIP_WHITESPACE;
		var_arg_idx--;
		if (!var_arg_idx){
			switch (st){
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
						*((sll_object_t***)ptr)=dt;
						*va_arg(va,sll_arg_count_t*)=all;
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
		switch (st){
			case 'b':
				if (!all){
					*((sll_bool_t*)ptr)=0;
					break;
				}
				*((sll_bool_t*)ptr)=sll_operator_bool(*al);
				al++;
				all--;
				break;
			case 'i':
				PARSE_TYPE(sll_integer_t,i,INT);
			case 'I':
				PARSE_TYPE_RANGE(sll_integer_t,sll_integer_t,i,INT);
			case 'f':
				PARSE_TYPE(sll_float_t,f,FLOAT);
			case 'F':
				PARSE_TYPE_RANGE(sll_integer_t,sll_integer_t,i,INT);
			case 'x':
				PARSE_TYPES(sll_int_float_t,INT,i,FLOAT,f,INIT_ZERO);
			case 'c':
				PARSE_TYPE(sll_char_t,c,CHAR);
			case 'C':
				PARSE_TYPE_RANGE(sll_char_t,__SLL_U32,c,CHAR);
			case 's':
				PARSE_TYPE_PTR(sll_string_t,s,STRING,SLL_INIT_STRING);
			case 'y':
				PARSE_TYPES(sll_char_string_t,CHAR,c,STRING,s,SLL_INIT_STRING);
			case 'a':
				PARSE_TYPE_PTR(sll_array_t,a,ARRAY,SLL_INIT_ARRAY);
			case 'm':
				PARSE_TYPE_PTR(sll_map_t,m,MAP,SLL_INIT_MAP);
			case 'o':
				if (!all){
					*((sll_object_t**)ptr)=SLL_ACQUIRE_STATIC_INT(0);
					break;
				}
				*((sll_object_t**)ptr)=*al;
				al++;
				all--;
				break;
		}
	}
	va_end(va);
}

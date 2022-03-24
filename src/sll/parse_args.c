#include <sll/_internal/common.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/map.h>
#include <sll/object.h>
#include <sll/operator.h>
#include <sll/parse_args.h>
#include <sll/static_object.h>
#include <sll/types.h>
#include <stdarg.h>



#define SKIP_WHITESPACE \
	while (*t==' '||(*t>8&&*t<14)){ \
		t++; \
	}

#define INIT_ZERO(x) (*(x)=0)

#define PARSE_TYPE(type,field,name,init) \
	{ \
		type* var=ptr; \
		if (!all){ \
			init(var); \
			break; \
		} \
		sll_object_t* obj=sll_operator_cast(*al,sll_static_int[SLL_OBJECT_TYPE_##name]); \
		*var=obj->dt.field; \
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
		if (SLL_OBJECT_GET_TYPE(*al)==SLL_OBJECT_TYPE_##name1){ \
			var->t=SLL_PARSE_ARGS_TYPE_##name1; \
			var->dt.field1=(*al)->dt.field1; \
		} \
		else if (SLL_OBJECT_GET_TYPE(*al)==SLL_OBJECT_TYPE_##name2){ \
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



__SLL_EXTERNAL void sll_parse_args(const sll_char_t* t,sll_bool_t var_arg,sll_object_t*const* al,sll_arg_count_t all,...){
	va_list va;
	va_start(va,all);
	SKIP_WHITESPACE;
	while (*t){
		void* ptr=va_arg(va,void*);
		sll_char_t st=*t;
		t++;
		SKIP_WHITESPACE;
		if (!(*t)&&var_arg){
			SLL_UNIMPLEMENTED();
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
				PARSE_TYPE(sll_integer_t,i,INT,INIT_ZERO);
			case 'f':
				PARSE_TYPE(sll_float_t,f,FLOAT,INIT_ZERO);
			case 'x':
				PARSE_TYPES(sll_int_float_t,INT,i,FLOAT,f,INIT_ZERO);
			case 'c':
				PARSE_TYPE(sll_char_t,c,CHAR,INIT_ZERO);
			case 's':
				PARSE_TYPE(sll_string_t,s,STRING,SLL_INIT_STRING);
			case 'y':
				PARSE_TYPES(sll_char_string_t,CHAR,c,STRING,s,SLL_INIT_STRING);
			case 'a':
				PARSE_TYPE(sll_array_t,a,ARRAY,SLL_INIT_ARRAY);
			case 'm':
				PARSE_TYPE(sll_map_t,m,MAP,SLL_INIT_MAP);
			case 'o':
				if (!all){
					*((sll_object_t**)ptr)=SLL_ACQUIRE_STATIC_INT(0);
					break;
				}
				SLL_ACQUIRE(*al);
				*((sll_object_t**)ptr)=*al;
				al++;
				all--;
				break;
		}
	}
	va_end(va);
}

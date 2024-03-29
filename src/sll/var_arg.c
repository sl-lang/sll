#include <sll/_internal/common.h>
#include <sll/_internal/var_arg.h>
#include <sll/common.h>
#include <sll/complex.h>
#include <sll/gc.h>
#include <sll/operator.h>
#include <sll/object.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/var_arg.h>
#include <stdarg.h>



#define GET_TYPE_IF_STRUCT(type_) \
	if (va->type==VAR_ARG_LIST_TYPE_STRUCT){ \
		SLL_ASSERT(va->data.struct_.offset_count); \
		sll_size_t off=*(va->data.struct_.offset_data); \
		va->data.struct_.offset_data++; \
		va->data.struct_.offset_count--; \
		return *((type_*)PTR(ADDR(va->data.struct_.base_pointer)+off)); \
	} \



sll_object_t _var_arg_converter(sll_var_arg_list_t* va){
	if (va->type==SLL_VAR_ARG_LIST_TYPE_C){
		return va_arg(*(va->data.c),converter_func_t)(va_arg(*(va->data.c),void*));
	}
	if (va->type==VAR_ARG_LIST_TYPE_STRUCT){
		SLL_ASSERT(va->data.struct_.converter_function_count&&va->data.struct_.offset_count);
		sll_object_t out=((converter_func_t)(*(va->data.struct_.converter_function_data)))(PTR(ADDR(va->data.struct_.base_pointer)+(*(va->data.struct_.offset_data))));
		va->data.struct_.converter_function_data++;
		va->data.struct_.converter_function_count--;
		va->data.struct_.offset_data++;
		va->data.struct_.offset_count--;
		return out;
	}
	SLL_UNIMPLEMENTED();
}



float _var_arg_get_float32(sll_var_arg_list_t* va){
	if (va->type==VAR_ARG_LIST_TYPE_STRUCT){
		SLL_ASSERT(va->data.struct_.offset_count);
		sll_size_t off=*(va->data.struct_.offset_data);
		va->data.struct_.offset_data++;
		va->data.struct_.offset_count--;
		return *((float*)PTR(ADDR(va->data.struct_.base_pointer)+off));
	}
	return (float)sll_var_arg_get_float(va);
}



addr_t _var_arg_get_pointer(sll_var_arg_list_t* va){
	if (va->type==SLL_VAR_ARG_LIST_TYPE_C){
		return va_arg(*(va->data.c),addr_t);
	}
	if (va->type==VAR_ARG_LIST_TYPE_STRUCT){
		SLL_ASSERT(va->data.struct_.offset_count);
		sll_size_t off=*(va->data.struct_.offset_data);
		va->data.struct_.offset_data++;
		va->data.struct_.offset_count--;
		return ADDR(va->data.struct_.base_pointer)+off;
	}
	if (!va->data.sll.count){
		return 0;
	}
	addr_t out=ADDR(*(va->data.sll.pointer));
	va->data.sll.pointer++;
	va->data.sll.count--;
	return out;
}



sll_size_t _var_arg_get_mask(sll_var_arg_list_t* va){
	sll_size_t mask;
	if (va->type==VAR_ARG_LIST_TYPE_STRUCT){
		mask=*(va->data.struct_.offset_data);
		va->data.struct_.offset_data++;
		va->data.struct_.offset_count--;
	}
	else{
		mask=(sll_size_t)sll_var_arg_get_int(va);
	}
	return sll_var_arg_get_int(va)&mask;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_var_arg_get(sll_var_arg_list_t* va){
	if (va->type==SLL_VAR_ARG_LIST_TYPE_C){
		return va_arg(*(va->data.c),void*);
	}
	GET_TYPE_IF_STRUCT(void*);
	if (!va->data.sll.count){
		return NULL;
	}
	void* out=*(va->data.sll.pointer);
	va->data.sll.pointer++;
	va->data.sll.count--;
	return out;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_char_t sll_var_arg_get_char(sll_var_arg_list_t* va){
	if (va->type==SLL_VAR_ARG_LIST_TYPE_C){
		return (sll_char_t)va_arg(*(va->data.c),int);
	}
	GET_TYPE_IF_STRUCT(sll_char_t);
	if (!va->data.sll.count){
		return 0;
	}
	sll_object_t n=sll_operator_cast((sll_object_t)(*(va->data.sll.pointer)),sll_static_int[SLL_OBJECT_TYPE_CHAR]);
	sll_char_t out=n->data.char_;
	SLL_RELEASE(n);
	va->data.sll.pointer++;
	va->data.sll.count--;
	return out;
}



__SLL_EXTERNAL void sll_var_arg_get_complex(sll_var_arg_list_t* va,sll_complex_t* out){
	if (va->type==SLL_VAR_ARG_LIST_TYPE_C){
		*out=*va_arg(*(va->data.c),sll_complex_t*);
	}
	else if (va->type==VAR_ARG_LIST_TYPE_STRUCT){
		SLL_ASSERT(va->data.struct_.offset_count);
		sll_size_t off=*(va->data.struct_.offset_data);
		va->data.struct_.offset_data++;
		va->data.struct_.offset_count--;
		*out=*((sll_complex_t*)PTR(ADDR(va->data.struct_.base_pointer)+off));
	}
	else if (!va->data.sll.count){
		SLL_INIT_COMPLEX(out);
	}
	else{
		sll_object_t n=sll_operator_cast((sll_object_t)(*(va->data.sll.pointer)),sll_static_int[SLL_OBJECT_TYPE_COMPLEX]);
		*out=n->data.complex_;
		SLL_RELEASE(n);
		va->data.sll.pointer++;
		va->data.sll.count--;
	}
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_float_t sll_var_arg_get_float(sll_var_arg_list_t* va){
	if (va->type==SLL_VAR_ARG_LIST_TYPE_C){
		return va_arg(*(va->data.c),sll_float_t);
	}
	GET_TYPE_IF_STRUCT(sll_float_t);
	if (!va->data.sll.count){
		return 0;
	}
	sll_object_t n=sll_operator_cast((sll_object_t)(*(va->data.sll.pointer)),sll_static_int[SLL_OBJECT_TYPE_FLOAT]);
	sll_float_t out=n->data.float_;
	SLL_RELEASE(n);
	va->data.sll.pointer++;
	va->data.sll.count--;
	return out;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_integer_t sll_var_arg_get_int(sll_var_arg_list_t* va){
	if (va->type==SLL_VAR_ARG_LIST_TYPE_C){
		return va_arg(*(va->data.c),sll_integer_t);
	}
	GET_TYPE_IF_STRUCT(sll_integer_t);
	if (!va->data.sll.count){
		return 0;
	}
	sll_object_t n=sll_operator_cast((sll_object_t)(*(va->data.sll.pointer)),sll_static_int[SLL_OBJECT_TYPE_INT]);
	sll_integer_t out=n->data.int_;
	SLL_RELEASE(n);
	va->data.sll.pointer++;
	va->data.sll.count--;
	return out;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t sll_var_arg_get_object(sll_var_arg_list_t* va){
	if (va->type==SLL_VAR_ARG_LIST_TYPE_SLL){
		if (!va->data.sll.count){
			return SLL_ACQUIRE_STATIC_INT(0);
		}
		sll_object_t out=*(va->data.sll.pointer);
		SLL_ACQUIRE(out);
		va->data.sll.pointer++;
		va->data.sll.count--;
		return out;
	}
	sll_object_t out;
	if (va->type==SLL_VAR_ARG_LIST_TYPE_C){
		out=va_arg(*(va->data.c),sll_object_t);
	}
	else{
		SLL_ASSERT(va->data.struct_.offset_count);
		sll_size_t off=*(va->data.struct_.offset_data);
		va->data.struct_.offset_data++;
		va->data.struct_.offset_count--;
		out=*((sll_object_t*)PTR(ADDR(va->data.struct_.base_pointer)+off));
	}
	if (!out){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	SLL_ACQUIRE(out);
	return out;
}



__SLL_EXTERNAL void sll_var_arg_get_string(sll_var_arg_list_t* va,sll_string_t* out){
	if (va->type==SLL_VAR_ARG_LIST_TYPE_C){
		sll_string_from_pointer(va_arg(*(va->data.c),const sll_char_t*),out);
	}
	else if (va->type==VAR_ARG_LIST_TYPE_STRUCT){
		SLL_UNREACHABLE();
	}
	else if (!va->data.sll.count){
		SLL_INIT_STRING(out);
	}
	else{
		sll_object_t n=sll_operator_cast((sll_object_t)(*(va->data.sll.pointer)),sll_static_int[SLL_OBJECT_TYPE_STRING]);
		sll_string_clone(&(n->data.string),out);
		SLL_RELEASE(n);
		va->data.sll.pointer++;
		va->data.sll.count--;
	}
}

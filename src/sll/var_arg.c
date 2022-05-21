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



#define GET_TYPE_IF_STRUCT(type) \
	if (va->t==VAR_ARG_LIST_TYPE_STRUCT){ \
		SLL_ASSERT(va->dt.s.l); \
		sll_size_t off=*(va->dt.s.off); \
		va->dt.s.off++; \
		va->dt.s.l--; \
		return *((type*)PTR(ADDR(va->dt.s.ptr)+off)); \
	} \



sll_object_t* _var_arg_converter(sll_var_arg_list_t* va){
	if (va->t==SLL_VAR_ARG_LIST_TYPE_C){
		return va_arg(*(va->dt.c),converter_func_t)(va_arg(*(va->dt.c),void*));
	}
	if (va->t==VAR_ARG_LIST_TYPE_STRUCT){
		SLL_ASSERT(va->dt.s.fnl&&va->dt.s.l);
		sll_object_t* o=((converter_func_t)(*(va->dt.s.fn)))(*((void**)PTR(ADDR(va->dt.s.ptr)+(*(va->dt.s.off)))));
		va->dt.s.fn++;
		va->dt.s.fnl--;
		va->dt.s.off++;
		va->dt.s.l--;
		return o;
	}
	SLL_UNIMPLEMENTED();
}



addr_t _var_arg_get_pointer(sll_var_arg_list_t* va){
	if (va->t==SLL_VAR_ARG_LIST_TYPE_C){
		return va_arg(*(va->dt.c),addr_t);
	}
	if (va->t==VAR_ARG_LIST_TYPE_STRUCT){
		SLL_ASSERT(va->dt.s.l);
		sll_size_t off=*(va->dt.s.off);
		va->dt.s.off++;
		va->dt.s.l--;
		return ADDR(va->dt.s.ptr)+off;
	}
	if (!va->dt.sll.l){
		return 0;
	}
	addr_t o=ADDR(*(va->dt.sll.p));
	va->dt.sll.p++;
	va->dt.sll.l--;
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_var_arg_get(sll_var_arg_list_t* va){
	if (va->t==SLL_VAR_ARG_LIST_TYPE_C){
		return va_arg(*(va->dt.c),void*);
	}
	GET_TYPE_IF_STRUCT(void*);
	if (!va->dt.sll.l){
		return NULL;
	}
	void* o=*(va->dt.sll.p);
	va->dt.sll.p++;
	va->dt.sll.l--;
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_char_t sll_var_arg_get_char(sll_var_arg_list_t* va){
	if (va->t==SLL_VAR_ARG_LIST_TYPE_C){
		return (sll_char_t)va_arg(*(va->dt.c),int);
	}
	GET_TYPE_IF_STRUCT(sll_char_t);
	if (!va->dt.sll.l){
		return 0;
	}
	sll_object_t* n=sll_operator_cast((sll_object_t*)(*(va->dt.sll.p)),sll_static_int[SLL_OBJECT_TYPE_CHAR]);
	sll_char_t o=n->dt.char_;
	SLL_RELEASE(n);
	va->dt.sll.p++;
	va->dt.sll.l--;
	return o;
}



__SLL_EXTERNAL void sll_var_arg_get_complex(sll_var_arg_list_t* va,sll_complex_t* o){
	if (va->t==SLL_VAR_ARG_LIST_TYPE_C){
		*o=*va_arg(*(va->dt.c),sll_complex_t*);
	}
	else if (va->t==VAR_ARG_LIST_TYPE_STRUCT){
		SLL_ASSERT(va->dt.s.l);
		sll_size_t off=*(va->dt.s.off);
		va->dt.s.off++;
		va->dt.s.l--;
		*o=*((sll_complex_t*)PTR(ADDR(va->dt.s.ptr)+off));
	}
	else if (!va->dt.sll.l){
		SLL_INIT_COMPLEX(o);
	}
	else{
		sll_object_t* n=sll_operator_cast((sll_object_t*)(*(va->dt.sll.p)),sll_static_int[SLL_OBJECT_TYPE_COMPLEX]);
		*o=n->dt.complex_;
		SLL_RELEASE(n);
		va->dt.sll.p++;
		va->dt.sll.l--;
	}
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_float_t sll_var_arg_get_float(sll_var_arg_list_t* va){
	if (va->t==SLL_VAR_ARG_LIST_TYPE_C){
		return va_arg(*(va->dt.c),sll_float_t);
	}
	GET_TYPE_IF_STRUCT(sll_float_t);
	if (!va->dt.sll.l){
		return 0;
	}
	sll_object_t* n=sll_operator_cast((sll_object_t*)(*(va->dt.sll.p)),sll_static_int[SLL_OBJECT_TYPE_FLOAT]);
	sll_float_t o=n->dt.float_;
	SLL_RELEASE(n);
	va->dt.sll.p++;
	va->dt.sll.l--;
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_integer_t sll_var_arg_get_int(sll_var_arg_list_t* va){
	if (va->t==SLL_VAR_ARG_LIST_TYPE_C){
		return va_arg(*(va->dt.c),sll_integer_t);
	}
	GET_TYPE_IF_STRUCT(sll_integer_t);
	if (!va->dt.sll.l){
		return 0;
	}
	sll_object_t* n=sll_operator_cast((sll_object_t*)(*(va->dt.sll.p)),sll_static_int[SLL_OBJECT_TYPE_INT]);
	sll_integer_t o=n->dt.int_;
	SLL_RELEASE(n);
	va->dt.sll.p++;
	va->dt.sll.l--;
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_var_arg_get_object(sll_var_arg_list_t* va){
	if (va->t==SLL_VAR_ARG_LIST_TYPE_SLL){
		if (!va->dt.sll.l){
			return SLL_ACQUIRE_STATIC_INT(0);
		}
		sll_object_t* o=*(va->dt.sll.p);
		SLL_ACQUIRE(o);
		va->dt.sll.p++;
		va->dt.sll.l--;
		return o;
	}
	sll_object_t* o;
	if (va->t==SLL_VAR_ARG_LIST_TYPE_C){
		o=va_arg(*(va->dt.c),sll_object_t*);
	}
	else{
		SLL_ASSERT(va->dt.s.l);
		sll_size_t off=*(va->dt.s.off);
		va->dt.s.off++;
		va->dt.s.l--;
		o=*((sll_object_t**)PTR(ADDR(va->dt.s.ptr)+off));
	}
	if (!o){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	SLL_ACQUIRE(o);
	return o;
}



__SLL_EXTERNAL void sll_var_arg_get_string(sll_var_arg_list_t* va,sll_string_t* o){
	if (va->t==SLL_VAR_ARG_LIST_TYPE_C){
		sll_string_from_pointer(va_arg(*(va->dt.c),const sll_char_t*),o);
	}
	else if (va->t==VAR_ARG_LIST_TYPE_STRUCT){
		SLL_UNREACHABLE();
	}
	else if (!va->dt.sll.l){
		SLL_INIT_STRING(o);
	}
	else{
		sll_object_t* n=sll_operator_cast((sll_object_t*)(*(va->dt.sll.p)),sll_static_int[SLL_OBJECT_TYPE_STRING]);
		sll_string_clone(&(n->dt.string),o);
		SLL_RELEASE(n);
		va->dt.sll.p++;
		va->dt.sll.l--;
	}
}

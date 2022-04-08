#include <sll/_internal/gc.h>
#include <sll/_internal/var_arg.h>
#include <sll/common.h>
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



addr_t _var_arg_get_pointer(sll_var_arg_list_t* va){
	if (va->t!=VAR_ARG_LIST_TYPE_STRUCT){
		SLL_UNREACHABLE();
	}
	SLL_ASSERT(va->dt.s.l);
	sll_size_t off=*(va->dt.s.off);
	va->dt.s.off++;
	va->dt.s.l--;
	return ADDR(va->dt.s.ptr)+off;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT const void* sll_var_arg_get(sll_var_arg_list_t* va){
	if (va->t==SLL_VAR_ARG_LIST_TYPE_C){
		return va_arg(*(va->dt.c),const void*);
	}
	GET_TYPE_IF_STRUCT(const void*);
	if (!va->dt.sll.l){
		return NULL;
	}
	const void* o=*(va->dt.sll.p);
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
	sll_char_t o=n->dt.c;
	GC_RELEASE(n);
	va->dt.sll.p++;
	va->dt.sll.l--;
	return o;
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
	sll_float_t o=n->dt.f;
	GC_RELEASE(n);
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
	sll_integer_t o=n->dt.i;
	GC_RELEASE(n);
	va->dt.sll.p++;
	va->dt.sll.l--;
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_var_arg_get_object(sll_var_arg_list_t* va){
	if (va->t==SLL_VAR_ARG_LIST_TYPE_C){
		sll_object_t* o=va_arg(*(va->dt.c),sll_object_t*);
		if (!o){
			return SLL_ACQUIRE_STATIC_INT(0);
		}
		SLL_ACQUIRE(o);
		return o;
	}
	if (va->t==VAR_ARG_LIST_TYPE_STRUCT){
		SLL_UNIMPLEMENTED();
	}
	if (!va->dt.sll.l){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_t* o=*(va->dt.sll.p);
	SLL_ACQUIRE(o);
	va->dt.sll.p++;
	va->dt.sll.l--;
	return o;
}



__SLL_EXTERNAL void sll_var_arg_get_string(sll_var_arg_list_t* va,sll_string_t* o){
	if (va->t==SLL_VAR_ARG_LIST_TYPE_C){
		sll_string_from_pointer(va_arg(*(va->dt.c),const sll_char_t*),o);
	}
	else if (va->t==VAR_ARG_LIST_TYPE_STRUCT){
		SLL_UNIMPLEMENTED();
	}
	else if (!va->dt.sll.l){
		SLL_INIT_STRING(o);
	}
	else{
		sll_object_t* n=sll_operator_cast((sll_object_t*)(*(va->dt.sll.p)),sll_static_int[SLL_OBJECT_TYPE_STRING]);
		sll_string_clone(&(n->dt.s),o);
		GC_RELEASE(n);
		va->dt.sll.p++;
		va->dt.sll.l--;
	}
}

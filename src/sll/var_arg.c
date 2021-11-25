#include <sll/common.h>
#include <sll/gc.h>
#include <sll/operator.h>
#include <sll/object.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/var_arg.h>
#include <stdarg.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT const void* sll_var_arg_get(sll_var_arg_list_t* va){
	if (va->t==SLL_VAR_ARG_LIST_TYPE_C){
		return va_arg(*(va->dt.c),void*);
	}
	if (!va->dt.sll.l){
		return 0;
	}
	const void* o=*(va->dt.sll.p);
	va->dt.sll.p++;
	va->dt.sll.l--;
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_integer_t sll_var_arg_get_int(sll_var_arg_list_t* va){
	if (va->t==SLL_VAR_ARG_LIST_TYPE_C){
		return va_arg(*(va->dt.c),sll_integer_t);
	}
	if (!va->dt.sll.l){
		return 0;
	}
	sll_object_t* n=sll_operator_cast((sll_object_t*)(*(va->dt.sll.p)),sll_static_int[SLL_OBJECT_TYPE_INT]);
	sll_integer_t o=n->dt.i;
	SLL_RELEASE(n);
	va->dt.sll.p++;
	va->dt.sll.l--;
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_float_t sll_var_arg_get_float(sll_var_arg_list_t* va){
	if (va->t==SLL_VAR_ARG_LIST_TYPE_C){
		return va_arg(*(va->dt.c),sll_float_t);
	}
	if (!va->dt.sll.l){
		return 0;
	}
	sll_object_t* n=sll_operator_cast((sll_object_t*)(*(va->dt.sll.p)),sll_static_int[SLL_OBJECT_TYPE_FLOAT]);
	sll_float_t o=n->dt.f;
	SLL_RELEASE(n);
	va->dt.sll.p++;
	va->dt.sll.l--;
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_char_t sll_var_arg_get_char(sll_var_arg_list_t* va){
	if (va->t==SLL_VAR_ARG_LIST_TYPE_C){
		return (sll_char_t)va_arg(*(va->dt.c),int);
	}
	if (!va->dt.sll.l){
		return 0;
	}
	sll_object_t* n=sll_operator_cast((sll_object_t*)(*(va->dt.sll.p)),sll_static_int[SLL_OBJECT_TYPE_CHAR]);
	sll_char_t o=n->dt.c;
	SLL_RELEASE(n);
	va->dt.sll.p++;
	va->dt.sll.l--;
	return o;
}



__SLL_EXTERNAL void sll_var_arg_get_string(sll_var_arg_list_t* va,sll_string_t* o){
	if (va->t==SLL_VAR_ARG_LIST_TYPE_C){
		sll_string_from_pointer(va_arg(*(va->dt.c),const sll_char_t*),o);
	}
	else if (!va->dt.sll.l){
		sll_string_create(0,o);
	}
	else{
		sll_object_t* n=sll_operator_cast((sll_object_t*)(*(va->dt.sll.p)),sll_static_int[SLL_OBJECT_TYPE_STRING]);
		sll_string_clone(&(n->dt.s),o);
		SLL_RELEASE(n);
		va->dt.sll.p++;
		va->dt.sll.l--;
	}
}

#include <sll/_internal/common.h>
#include <sll/_internal/complex.h>
#include <sll/common.h>
#include <sll/object.h>
#include <sll/static_object.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_len(sll_object_t* a){
	switch (a->t){
		case SLL_OBJECT_TYPE_INT:
			return sll_int_to_object(a->dt.i);
		case SLL_OBJECT_TYPE_FLOAT:
			return sll_float_to_object(a->dt.f);
		case SLL_OBJECT_TYPE_CHAR:
			return sll_int_to_object(a->dt.c);
		case SLL_OBJECT_TYPE_COMPLEX:
			return sll_float_to_object(COMPLEX_ABS(a->dt.d));
		case SLL_OBJECT_TYPE_STRING:
			return sll_int_to_object(a->dt.s.l);
		case SLL_OBJECT_TYPE_ARRAY:
			return sll_int_to_object(a->dt.a.l);
		case SLL_OBJECT_TYPE_MAP:
			return sll_int_to_object(a->dt.m.l);
		default:
			SLL_UNREACHABLE();
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}

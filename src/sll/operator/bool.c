#include <sll/common.h>
#include <sll/gc.h>
#include <sll/object.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_operator_bool(sll_object_t* a){
	switch (a->t){
		case SLL_OBJECT_TYPE_INT:
			return !!a->dt.i;
		case SLL_OBJECT_TYPE_FLOAT:
			return !!a->dt.f;
		case SLL_OBJECT_TYPE_CHAR:
			return !!a->dt.c;
		case SLL_OBJECT_TYPE_COMPLEX:
			return (!!a->dt.d.real)|(!!a->dt.d.imag);
		case SLL_OBJECT_TYPE_STRING:
			return !!a->dt.s.l;
		case SLL_OBJECT_TYPE_ARRAY:
			return !!a->dt.a.length;
		case SLL_OBJECT_TYPE_MAP:
			return !!a->dt.m.length;
	}
	return 1;
}

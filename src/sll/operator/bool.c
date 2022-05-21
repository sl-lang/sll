#include <sll/common.h>
#include <sll/gc.h>
#include <sll/object.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_operator_bool(sll_object_t* a){
	switch (a->t){
		case SLL_OBJECT_TYPE_INT:
			return !!a->dt.int_;
		case SLL_OBJECT_TYPE_FLOAT:
			return !!a->dt.float_;
		case SLL_OBJECT_TYPE_CHAR:
			return !!a->dt.char_;
		case SLL_OBJECT_TYPE_COMPLEX:
			return (!!a->dt.complex_.real)|(!!a->dt.complex_.imag);
		case SLL_OBJECT_TYPE_STRING:
			return !!a->dt.string.l;
		case SLL_OBJECT_TYPE_ARRAY:
			return !!a->dt.array.length;
		case SLL_OBJECT_TYPE_MAP:
			return !!a->dt.map.length;
	}
	return 1;
}

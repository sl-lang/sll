#include <sll/common.h>
#include <sll/gc.h>
#include <sll/object.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_operator_bool(sll_object_t* a){
	switch (a->type){
		case SLL_OBJECT_TYPE_INT:
			return !!a->data.int_;
		case SLL_OBJECT_TYPE_FLOAT:
			return !!a->data.float_;
		case SLL_OBJECT_TYPE_CHAR:
			return !!a->data.char_;
		case SLL_OBJECT_TYPE_COMPLEX:
			return (!!a->data.complex_.real)|(!!a->data.complex_.imag);
		case SLL_OBJECT_TYPE_STRING:
			return !!a->data.string.l;
		case SLL_OBJECT_TYPE_ARRAY:
			return !!a->data.array.length;
		case SLL_OBJECT_TYPE_MAP:
			return !!a->data.map.length;
	}
	return 1;
}

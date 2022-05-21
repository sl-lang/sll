#include <sll/_internal/common.h>
#include <sll/_internal/complex.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/object.h>
#include <sll/static_object.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_len(sll_object_t* a){
	switch (a->type){
		case SLL_OBJECT_TYPE_INT:
			return sll_int_to_object(a->data.int_);
		case SLL_OBJECT_TYPE_FLOAT:
			return sll_float_to_object(a->data.float_);
		case SLL_OBJECT_TYPE_CHAR:
			return sll_int_to_object(a->data.char_);
		case SLL_OBJECT_TYPE_COMPLEX:
			return sll_float_to_object(COMPLEX_ABS(a->data.complex_));
		case SLL_OBJECT_TYPE_STRING:
			return sll_int_to_object(a->data.string.l);
		case SLL_OBJECT_TYPE_ARRAY:
			return sll_int_to_object(a->data.array.length);
		case SLL_OBJECT_TYPE_MAP:
			return sll_int_to_object(a->data.map.length);
		default:
			SLL_UNREACHABLE();
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}

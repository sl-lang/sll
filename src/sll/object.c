#include <sll/_sll_internal.h>
#include <sll/core.h>
#include <sll/common.h>
#include <sll/types.h>



__SLL_FUNC __SLL_RETURN_COMPARE sll_compare_runtime_object(const sll_runtime_object_t* a,const sll_runtime_object_t* b){
	switch (SLL_RUNTIME_OBJECT_GET_TYPE(a)){
		case SLL_RUNTIME_OBJECT_TYPE_INT:
			{
				int64_t v;
				switch (SLL_RUNTIME_OBJECT_GET_TYPE(b)){
					case SLL_RUNTIME_OBJECT_TYPE_INT:
						v=b->dt.i;
						break;
					case SLL_RUNTIME_OBJECT_TYPE_FLOAT:
						return (b->dt.f>a->dt.i?SLL_COMPARE_RESULT_BELOW:(b->dt.f<a->dt.i?SLL_COMPARE_RESULT_ABOVE:SLL_COMPARE_RESULT_EQUAL));
					case SLL_RUNTIME_OBJECT_TYPE_CHAR:
						v=b->dt.c;
						break;
					case SLL_RUNTIME_OBJECT_TYPE_STRING:
						v=b->dt.s->l;
						break;
					default:
						return SLL_COMPARE_RESULT_ERROR;
				}
				return (v>a->dt.i?SLL_COMPARE_RESULT_BELOW:(v<a->dt.i?SLL_COMPARE_RESULT_ABOVE:SLL_COMPARE_RESULT_EQUAL));
			}
		case SLL_RUNTIME_OBJECT_TYPE_FLOAT:
			ASSERT(!"Unimplemented");
		case SLL_RUNTIME_OBJECT_TYPE_CHAR:
			ASSERT(!"Unimplemented");
		case SLL_RUNTIME_OBJECT_TYPE_STRING:
			ASSERT(!"Unimplemented");
		default:
			return SLL_COMPARE_RESULT_ERROR;
	}
}



__SLL_FUNC __SLL_RETURN_COMPARE sll_runtime_object_nonzero(const sll_runtime_object_t* o){
	switch (SLL_RUNTIME_OBJECT_GET_TYPE(o)){
		case SLL_RUNTIME_OBJECT_TYPE_INT:
			return (o->dt.i?SLL_COMPARE_RESULT_NONZERO:SLL_COMPARE_RESULT_ZERO);
		case SLL_RUNTIME_OBJECT_TYPE_FLOAT:
			return (o->dt.f?SLL_COMPARE_RESULT_NONZERO:SLL_COMPARE_RESULT_ZERO);
		case SLL_RUNTIME_OBJECT_TYPE_CHAR:
			return (o->dt.c?SLL_COMPARE_RESULT_NONZERO:SLL_COMPARE_RESULT_ZERO);
		case SLL_RUNTIME_OBJECT_TYPE_STRING:
			return (o->dt.s->l?SLL_COMPARE_RESULT_NONZERO:SLL_COMPARE_RESULT_ZERO);
		default:
			return SLL_COMPARE_RESULT_ERROR;
	}
}

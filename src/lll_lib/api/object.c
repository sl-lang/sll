#include <lll_lib.h>
#include <lll_lib_api.h>
#include <_lll_lib_internal.h>



__LLL_IMPORT_EXPORT __LLL_RETURN_COMPARE lll_compare_runtime_object(const lll_runtime_object_t* a,const lll_runtime_object_t* b){
	switch (LLL_RUNTIME_OBJECT_GET_TYPE(a)){
		case LLL_RUNTIME_OBJECT_TYPE_INT:
			{
				int64_t v;
				switch (LLL_RUNTIME_OBJECT_GET_TYPE(b)){
					case LLL_RUNTIME_OBJECT_TYPE_INT:
						v=b->dt.i;
						break;
					case LLL_RUNTIME_OBJECT_TYPE_FLOAT:
						return (b->dt.f>a->dt.i?LLL_COMPARE_RESULT_BELOW:(b->dt.f<a->dt.i?LLL_COMPARE_RESULT_ABOVE:LLL_COMPARE_RESULT_EQUAL));
					case LLL_RUNTIME_OBJECT_TYPE_CHAR:
						v=b->dt.c;
						break;
					case LLL_RUNTIME_OBJECT_TYPE_STRING:
						v=b->dt.s->l;
						break;
					default:
						return LLL_COMPARE_RESULT_ERROR;
				}
				return (v>a->dt.i?LLL_COMPARE_RESULT_BELOW:(v<a->dt.i?LLL_COMPARE_RESULT_ABOVE:LLL_COMPARE_RESULT_EQUAL));
			}
		case LLL_RUNTIME_OBJECT_TYPE_FLOAT:
			ASSERT(!"Unimplemented");
		case LLL_RUNTIME_OBJECT_TYPE_CHAR:
			ASSERT(!"Unimplemented");
		case LLL_RUNTIME_OBJECT_TYPE_STRING:
			ASSERT(!"Unimplemented");
		default:
			return LLL_COMPARE_RESULT_ERROR;
	}
}



__LLL_IMPORT_EXPORT __LLL_RETURN_COMPARE lll_runtime_object_nonzero(const lll_runtime_object_t* o){
	switch (LLL_RUNTIME_OBJECT_GET_TYPE(o)){
		case LLL_RUNTIME_OBJECT_TYPE_INT:
			return (o->dt.i?LLL_COMPARE_RESULT_NONZERO:LLL_COMPARE_RESULT_ZERO);
		case LLL_RUNTIME_OBJECT_TYPE_FLOAT:
			return (o->dt.f?LLL_COMPARE_RESULT_NONZERO:LLL_COMPARE_RESULT_ZERO);
		case LLL_RUNTIME_OBJECT_TYPE_CHAR:
			return (o->dt.c?LLL_COMPARE_RESULT_NONZERO:LLL_COMPARE_RESULT_ZERO);
		case LLL_RUNTIME_OBJECT_TYPE_STRING:
			return (o->dt.s->l?LLL_COMPARE_RESULT_NONZERO:LLL_COMPARE_RESULT_ZERO);
		default:
			return LLL_COMPARE_RESULT_ERROR;
	}
}

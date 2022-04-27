#include <sll/_internal/gc.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/map.h>
#include <sll/object.h>
#include <sll/operator.h>
#include <sll/static_object.h>
#include <sll/types.h>
#include <sll/vm.h>



__SLL_EXTERNAL void sll_operator_assign(sll_object_t* a,sll_object_t* b,sll_object_t* v){
	if (a->t==SLL_OBJECT_TYPE_STRING){
		if (b->t==SLL_OBJECT_TYPE_INT){
			sll_object_t* tmp=sll_operator_cast(v,sll_static_int[SLL_OBJECT_TYPE_CHAR]);
			SLL_ASSERT(tmp->t==SLL_OBJECT_TYPE_CHAR);
			sll_integer_t idx=b->dt.i-b->dt.i/a->dt.s.l*a->dt.s.l;
			if (idx<0){
				idx+=a->dt.s.l;
			}
			sll_string_set_char(tmp->dt.c,(sll_string_length_t)idx,&(a->dt.s));
			SLL_RELEASE(tmp);
		}
	}
	else if (a->t==SLL_OBJECT_TYPE_ARRAY){
		if (b->t==SLL_OBJECT_TYPE_INT){
			sll_integer_t idx=b->dt.i-b->dt.i/a->dt.a.l*a->dt.a.l;
			if (idx<0){
				idx+=a->dt.a.l;
			}
			sll_array_set(&(a->dt.a),(sll_array_length_t)idx,v);
		}
	}
	else if (a->t==SLL_OBJECT_TYPE_MAP){
		sll_map_set(&(a->dt.m),b,v);
	}
	else if (sll_current_runtime_data&&b->t==SLL_OBJECT_TYPE_STRING&&a->t>SLL_MAX_OBJECT_TYPE&&a->t<=sll_current_runtime_data->tt->l+SLL_MAX_OBJECT_TYPE){
		sll_object_set_field(sll_current_runtime_data->tt,a,&(b->dt.s),v);
	}
}



__SLL_EXTERNAL void sll_operator_assign_range(sll_object_t* a,sll_object_t* b,sll_object_t* c,sll_object_t* v){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL void sll_operator_assign_range_step(sll_object_t* a,sll_object_t* b,sll_object_t* c,sll_object_t* d,sll_object_t* v){
	SLL_UNIMPLEMENTED();
}

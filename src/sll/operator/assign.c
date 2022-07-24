#include <sll/_internal/common.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/map.h>
#include <sll/object.h>
#include <sll/operator.h>
#include <sll/static_object.h>
#include <sll/types.h>
#include <sll/vm.h>



__SLL_EXTERNAL void sll_operator_assign(sll_object_t a,sll_object_t b,sll_object_t v){
	if (a->type==SLL_OBJECT_TYPE_STRING){
		if (b->type==SLL_OBJECT_TYPE_INT){
			sll_object_t tmp=sll_operator_cast(v,sll_static_int[SLL_OBJECT_TYPE_CHAR]);
			SLL_ASSERT(tmp->type==SLL_OBJECT_TYPE_CHAR);
			sll_integer_t idx=b->data.int_-b->data.int_/a->data.string.length*a->data.string.length;
			if (idx<0){
				idx+=a->data.string.length;
			}
			sll_string_set_char(&(a->data.string),tmp->data.char_,(sll_string_length_t)idx);
			SLL_RELEASE(tmp);
		}
	}
	else if (a->type==SLL_OBJECT_TYPE_ARRAY){
		if (b->type==SLL_OBJECT_TYPE_INT){
			sll_integer_t idx=b->data.int_-b->data.int_/a->data.array.length*a->data.array.length;
			if (idx<0){
				idx+=a->data.array.length;
			}
			sll_array_set(&(a->data.array),(sll_array_length_t)idx,v);
		}
	}
	else if (a->type==SLL_OBJECT_TYPE_MAP){
		sll_map_set(&(a->data.map),b,v);
	}
	else if (sll_current_runtime_data&&b->type==SLL_OBJECT_TYPE_STRING&&a->type>SLL_MAX_OBJECT_TYPE&&a->type<=sll_current_runtime_data->type_table->length+SLL_MAX_OBJECT_TYPE){
		sll_object_set_field(sll_current_runtime_data->type_table,a,&(b->data.string),v);
	}
}



__SLL_EXTERNAL void sll_operator_assign_range(sll_object_t a,sll_object_t b,sll_object_t c,sll_object_t v){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL void sll_operator_assign_range_step(sll_object_t a,sll_object_t b,sll_object_t c,sll_object_t d,sll_object_t v){
	SLL_UNIMPLEMENTED();
}

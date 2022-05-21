#include <sll/_internal/common.h>
#include <sll/_internal/static_object.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/object.h>
#include <sll/static_object.h>
#include <sll/types.h>
#include <sll/vm.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_copy(sll_object_t* a,sll_bool_t d){
	switch (a->type){
		case SLL_OBJECT_TYPE_INT:
		case SLL_OBJECT_TYPE_FLOAT:
		case SLL_OBJECT_TYPE_CHAR:
		case SLL_OBJECT_TYPE_COMPLEX:
			SLL_ACQUIRE(a);
			return a;
		case SLL_OBJECT_TYPE_STRING:
			return STRING_TO_OBJECT(&(a->data.string));
		case SLL_OBJECT_TYPE_ARRAY:
			{
				sll_object_t* o=sll_array_length_to_object(a->data.array.length);
				for (sll_map_length_t i=0;i<a->data.array.length;i++){
					if (d){
						o->data.array.data[i]=sll_operator_copy(a->data.array.data[i],1);
					}
					else{
						o->data.array.data[i]=a->data.array.data[i];
						SLL_ACQUIRE(o->data.array.data[i]);
					}
				}
				return o;
			}
		case SLL_OBJECT_TYPE_MAP:
			{
				sll_object_t* o=sll_map_length_to_object(a->data.map.length);
				for (sll_map_length_t i=0;i<(a->data.map.length<<1);i++){
					if (d){
						o->data.map.data[i]=sll_operator_copy(a->data.map.data[i],1);
					}
					else{
						o->data.map.data[i]=a->data.map.data[i];
						SLL_ACQUIRE(o->data.map.data[i]);
					}
				}
				return o;
			}
	}
	SLL_ASSERT(a->type>SLL_MAX_OBJECT_TYPE);
	if (sll_current_runtime_data&&a->type<=sll_current_runtime_data->tt->l+SLL_MAX_OBJECT_TYPE){
		return sll_object_clone(sll_current_runtime_data->tt,a,d);
	}
	SLL_ACQUIRE(a);
	return a;
}

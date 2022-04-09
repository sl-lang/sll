#include <sll/_internal/common.h>
#include <sll/common.h>
#include <sll/object.h>
#include <sll/static_object.h>
#include <sll/types.h>
#include <sll/vm.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_copy(sll_object_t* a,sll_bool_t d){
	switch (a->t){
		case SLL_OBJECT_TYPE_INT:
		case SLL_OBJECT_TYPE_FLOAT:
		case SLL_OBJECT_TYPE_CHAR:
			SLL_ACQUIRE(a);
			return a;
		case SLL_OBJECT_TYPE_STRING:
			return sll_string_to_object(&(a->dt.s));
		case SLL_OBJECT_TYPE_ARRAY:
			{
				sll_object_t* o=sll_array_length_to_object(a->dt.a.l);
				for (sll_map_length_t i=0;i<a->dt.a.l;i++){
					if (d){
						o->dt.a.v[i]=sll_operator_copy(a->dt.a.v[i],1);
					}
					else{
						o->dt.a.v[i]=a->dt.a.v[i];
						SLL_ACQUIRE(o->dt.a.v[i]);
					}
				}
				return o;
			}
		case SLL_OBJECT_TYPE_MAP:
			{
				sll_object_t* o=sll_map_length_to_object(a->dt.m.l);
				for (sll_map_length_t i=0;i<(a->dt.m.l<<1);i++){
					if (d){
						o->dt.m.v[i]=sll_operator_copy(a->dt.m.v[i],1);
					}
					else{
						o->dt.m.v[i]=a->dt.m.v[i];
						SLL_ACQUIRE(o->dt.m.v[i]);
					}
				}
				return o;
			}
	}
	SLL_ASSERT(a->t>SLL_MAX_OBJECT_TYPE);
	if (sll_current_runtime_data&&a->t<=sll_current_runtime_data->tt->l+SLL_MAX_OBJECT_TYPE){
		return sll_object_clone(sll_current_runtime_data->tt,a,d);
	}
	SLL_ACQUIRE(a);
	return a;
}

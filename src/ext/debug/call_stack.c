#include <debug/util.h>
#include <sll.h>



sll_object_t* debug_get_call_stack(sll_object_t*const* al,sll_arg_count_t all){
	if (!debug_cs_type||!debug_cs_raw_type){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_t* o=SLL_CREATE();
	o->t=SLL_OBJECT_TYPE_ARRAY;
	sll_array_create(0,&(o->dt.a));
	return o;
}

#include <debug/util.h>
#include <sll.h>



sll_object_t* debug_get_call_stack(sll_object_t*const* al,sll_arg_count_t all){
	if (!debug_cs_type||!debug_cs_raw_type){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_t* o=SLL_CREATE();
	o->t=SLL_OBJECT_TYPE_ARRAY;
	const sll_call_stack_t* c_st=sll_get_call_stack();
	sll_array_create(c_st->l-1,&(o->dt.a));
	for (sll_call_stack_size_t i=0;i<c_st->l-1;i++){
		const sll_call_stack_frame_t* k=c_st->dt+i;
		sll_object_t* dt_raw[2]={
			SLL_FROM_INT(k->_ii),
			SLL_FROM_INT(k->_s)
		};
		sll_object_t* nm=SLL_CREATE();
		nm->t=SLL_OBJECT_TYPE_STRING;
		sll_string_from_pointer(k->nm,&(nm->dt.s));
		sll_object_t* dt[3]={
			nm,
			ii_to_loc(k->_ii),
			sll_create_object_type(sll_current_runtime_data->tt,debug_cs_raw_type,dt_raw,2)
		};
		SLL_RELEASE(dt_raw[0]);
		SLL_RELEASE(dt_raw[1]);
		o->dt.a.v[i]=sll_create_object_type(sll_current_runtime_data->tt,debug_cs_type,dt,3);
		SLL_RELEASE(nm);
		SLL_RELEASE(dt[1]);
		SLL_RELEASE(dt[2]);
	}
	return o;
}

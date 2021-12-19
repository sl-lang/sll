#include <sll_ext/common.h>
#include <sll.h>



static sll_object_type_t _debug_type=0;



__SLL_EXT_EXTERNAL sll_object_t* sll_ext_api_debug_get_call_stack(const sll_object_t*const* al,sll_arg_count_t all){
	if (!sll_current_runtime_data||!_debug_type){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_t* o=SLL_CREATE();
	o->t=SLL_OBJECT_TYPE_ARRAY;
	sll_array_create(sll_current_runtime_data->c_st->l,&(o->dt.a));
	for (sll_call_stack_size_t i=0;i<sll_current_runtime_data->c_st->l;i++){
		sll_object_t* nm=SLL_CREATE();
		nm->t=SLL_OBJECT_TYPE_STRING;
		sll_string_clone(sll_current_runtime_data->a_dt->st.dt+(sll_current_runtime_data->c_st->dt+i)->nm,&(nm->dt.s));
		o->dt.a.v[i]=sll_create_object_type(sll_current_runtime_data->tt,_debug_type,&nm,1);
		SLL_RELEASE(nm);
	}
	return o;
}



__SLL_EXT_EXTERNAL sll_object_t* sll_ext_api_debug_set_type(const sll_object_t*const* al,sll_arg_count_t all){
	if (all){
		const sll_object_t* a=*al;
		if (a->t==SLL_OBJECT_TYPE_INT){
			_debug_type=(sll_object_type_t)(a->dt.i);
		}
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}

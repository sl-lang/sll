#include <sll_ext/common.h>
#include <sll_ext/debug.h>
#include <sll.h>



static sll_object_type_t _debug_type=0;



static sll_object_t* _debug_set_type(const sll_object_t*const* al,sll_arg_count_t all){
	if (all){
		const sll_object_t* a=*al;
		if (a->t==SLL_OBJECT_TYPE_INT){
			_debug_type=(sll_object_type_t)(a->dt.i);
		}
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



void _register_debug_functions(void){
	sll_register_internal_function(sll_current_runtime_data->ift,SLL_CHAR("sll_ext:debug_get_call_stack"),sll_ext_api_debug_get_call_stack,0);
	sll_register_internal_function(sll_current_runtime_data->ift,SLL_CHAR("sll_ext:debug_set_type"),_debug_set_type,0);
}



__SLL_EXT_EXTERNAL sll_object_t* sll_ext_api_debug_get_call_stack(const sll_object_t*const* al,sll_arg_count_t all){
	if (!sll_current_runtime_data||!_debug_type){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_t* o=SLL_CREATE();
	o->t=SLL_OBJECT_TYPE_ARRAY;
	sll_array_create(sll_current_runtime_data->c_st->l,&(o->dt.a));
	for (sll_call_stack_size_t i=0;i<sll_current_runtime_data->c_st->l;i++){
		const sll_call_stack_frame_t* k=sll_current_runtime_data->c_st->dt+i;
		sll_object_t* nm=SLL_CREATE();
		nm->t=SLL_OBJECT_TYPE_STRING;
		sll_string_clone(sll_current_runtime_data->a_dt->st.dt+k->nm,&(nm->dt.s));
		sll_object_t* dt[]={
			nm,
			SLL_FROM_INT(k->_ii),
			SLL_FROM_INT(k->_s)
		};
		o->dt.a.v[i]=sll_create_object_type(sll_current_runtime_data->tt,_debug_type,dt,3);
		SLL_RELEASE(nm);
		SLL_RELEASE(dt[1]);
		SLL_RELEASE(dt[2]);
	}
	return o;
}

#include <sll_ext/common.h>
#include <sll.h>



static sll_object_type_t _debug_cs_type=0;
static sll_object_type_t _debug_vm_cfg_type=0;



static sll_object_t* _debug_get_call_stack(const sll_object_t*const* al,sll_arg_count_t all){
	if (!_debug_cs_type){
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
		sll_object_t* dt[3]={
			nm,
			SLL_FROM_INT(k->_ii),
			SLL_FROM_INT(k->_s)
		};
		o->dt.a.v[i]=sll_create_object_type(sll_current_runtime_data->tt,_debug_cs_type,dt,3);
		SLL_RELEASE(nm);
		SLL_RELEASE(dt[1]);
		SLL_RELEASE(dt[2]);
	}
	return o;
}



static sll_object_t* _debug_get_instruction_count(const sll_object_t*const* al,sll_arg_count_t all){
	return SLL_FROM_INT(sll_current_instruction_count);
}



static sll_object_t* _debug_get_name(const sll_object_t*const* al,sll_arg_count_t all){
	if (!all){
		return SLL_ACQUIRE_STATIC(str_zero);
	}
	sll_object_t* o=SLL_CREATE();
	o->t=SLL_OBJECT_TYPE_STRING;
	const sll_object_t* v=*al;
	if (SLL_OBJECT_GET_TYPE(v)==SLL_OBJECT_TYPE_INT){
		if (v->dt.i<0){
			sll_function_index_t i=(sll_function_index_t)(~v->dt.i);
			if (i<sll_current_runtime_data->ift->l){
				sll_string_clone(&((*(sll_current_runtime_data->ift->dt+i))->nm),&(o->dt.s));
				return o;
			}
		}
		else if (v->dt.i&&v->dt.i<=sll_current_runtime_data->a_dt->ft.l){
			sll_string_clone(sll_current_runtime_data->a_dt->st.dt+(sll_current_runtime_data->a_dt->ft.dt+v->dt.i-1)->nm,&(o->dt.s));
			return o;
		}
	}
	else if (SLL_OBJECT_GET_TYPE(v)>SLL_MAX_OBJECT_TYPE&&SLL_OBJECT_GET_TYPE(v)-SLL_MAX_OBJECT_TYPE-1<sll_current_runtime_data->tt->l){
		sll_string_clone(&((*(sll_current_runtime_data->tt->dt+SLL_OBJECT_GET_TYPE(v)-SLL_MAX_OBJECT_TYPE-1))->nm),&(o->dt.s));
		return o;
	}
	SLL_INIT_STRING(&(o->dt.s));
	return o;
}



static sll_object_t* _debug_get_ref_count(const sll_object_t*const* al,sll_arg_count_t all){
	if (!all){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	return SLL_FROM_INT((*al)->rc);
}



static sll_object_t* _debug_get_vm_config(const sll_object_t*const* al,sll_arg_count_t all){
	if (!_debug_vm_cfg_type){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_t* dt[4]={
		SLL_FROM_INT(sll_current_vm_config->s_sz),
		SLL_FROM_FILE(sll_current_vm_config->in),
		SLL_FROM_FILE(sll_current_vm_config->out),
		SLL_FROM_FILE(sll_current_vm_config->err)
	};
	sll_object_t* o=sll_create_object_type(sll_current_runtime_data->tt,_debug_vm_cfg_type,dt,4);
	SLL_RELEASE(dt[0]);
	SLL_RELEASE(dt[1]);
	SLL_RELEASE(dt[2]);
	SLL_RELEASE(dt[3]);
	return o;
}



static sll_object_t* _debug__set_type(const sll_object_t*const* al,sll_arg_count_t all){
	if (all>1){
		const sll_object_t* a=*al;
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_INT){
			_debug_cs_type=(sll_object_type_t)(a->dt.i);
		}
		a=*(al+1);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_INT){
			_debug_vm_cfg_type=(sll_object_type_t)(a->dt.i);
		}
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



void _register_debug_functions(void){
	sll_register_internal_function(sll_current_runtime_data->ift,SLL_CHAR("sll_ext:debug_get_call_stack"),_debug_get_call_stack,0);
	sll_register_internal_function(sll_current_runtime_data->ift,SLL_CHAR("sll_ext:debug_get_instruction_count"),_debug_get_instruction_count,0);
	sll_register_internal_function(sll_current_runtime_data->ift,SLL_CHAR("sll_ext:debug_get_name"),_debug_get_name,0);
	sll_register_internal_function(sll_current_runtime_data->ift,SLL_CHAR("sll_ext:debug_get_ref_count"),_debug_get_ref_count,0);
	sll_register_internal_function(sll_current_runtime_data->ift,SLL_CHAR("sll_ext:debug_get_vm_config"),_debug_get_vm_config,0);
	sll_register_internal_function(sll_current_runtime_data->ift,SLL_CHAR("sll_ext:debug__set_type"),_debug__set_type,0);
}

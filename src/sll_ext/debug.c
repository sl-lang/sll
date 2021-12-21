#include <sll_ext/common.h>
#include <sll.h>



static sll_object_type_t _debug_cs_type=0;
static sll_object_type_t _debug_cs_raw_type=0;
static sll_object_type_t _debug_loc_type=0;
static sll_object_type_t _debug_vm_cfg_type=0;



static sll_object_t* _ii_to_loc(sll_instruction_index_t ii){
	if (!_debug_loc_type){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_string_index_t fp_i;
	sll_string_index_t fn_i;
	sll_file_offset_t ln=sll_get_location(sll_current_runtime_data->a_dt,ii,&fp_i,&fn_i);
	sll_object_t* fp=SLL_CREATE();
	fp->t=SLL_OBJECT_TYPE_STRING;
	sll_string_clone(sll_current_runtime_data->a_dt->st.dt+fp_i,&(fp->dt.s));
	sll_object_t* fn=SLL_CREATE();
	fn->t=SLL_OBJECT_TYPE_STRING;
	if (fn_i==SLL_MAX_STRING_INDEX){
		sll_string_from_pointer(SLL_CHAR("@code"),&(fn->dt.s));
	}
	else{
		sll_string_clone(sll_current_runtime_data->a_dt->st.dt+fn_i,&(fn->dt.s));
	}
	sll_object_t* dt[3]={
		fp,
		fn,
		SLL_FROM_INT(ln)
	};
	sll_object_t* o=sll_create_object_type(sll_current_runtime_data->tt,_debug_loc_type,dt,3);
	SLL_RELEASE(fp);
	SLL_RELEASE(fn);
	SLL_RELEASE(dt[2]);
	return o;
}



static sll_object_t* _debug__set_type(const sll_object_t*const* al,sll_arg_count_t all){
	if (all>2){
		const sll_object_t* a=*al;
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_INT){
			_debug_cs_type=(sll_object_type_t)(a->dt.i);
		}
		a=*(al+1);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_INT){
			_debug_cs_raw_type=(sll_object_type_t)(a->dt.i);
		}
		a=*(al+2);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_INT){
			_debug_loc_type=(sll_object_type_t)(a->dt.i);
		}
		a=*(al+3);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_INT){
			_debug_vm_cfg_type=(sll_object_type_t)(a->dt.i);
		}
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



static sll_object_t* _debug_get_call_stack(const sll_object_t*const* al,sll_arg_count_t all){
	if (!_debug_cs_type){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_t* o=SLL_CREATE();
	o->t=SLL_OBJECT_TYPE_ARRAY;
	sll_array_create(sll_current_runtime_data->c_st->l,&(o->dt.a));
	for (sll_call_stack_size_t i=0;i<sll_current_runtime_data->c_st->l;i++){
		const sll_call_stack_frame_t* k=sll_current_runtime_data->c_st->dt+i;
		sll_object_t* dt_raw[2]={
			SLL_FROM_INT(k->_ii),
			SLL_FROM_INT(k->_s)
		};
		sll_object_t* nm=SLL_CREATE();
		nm->t=SLL_OBJECT_TYPE_STRING;
		sll_string_clone(sll_current_runtime_data->a_dt->st.dt+k->nm,&(nm->dt.s));
		sll_object_t* dt[3]={
			nm,
			_ii_to_loc(k->_ii),
			sll_create_object_type(sll_current_runtime_data->tt,_debug_cs_raw_type,dt_raw,2)
		};
		SLL_RELEASE(dt_raw[0]);
		SLL_RELEASE(dt_raw[1]);
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



static sll_object_t* _debug_get_instruction_index(const sll_object_t*const* al,sll_arg_count_t all){
	return SLL_FROM_INT(sll_current_instruction_index);
}



static sll_object_t* _debug_get_location(const sll_object_t*const* al,sll_arg_count_t all){
	sll_instruction_index_t ii=sll_current_instruction_index;
	if (all){
		sll_object_t* ii_o=sll_operator_cast((sll_object_t*)(*al),sll_static_int[SLL_OBJECT_TYPE_INT]);
		ii=(ii_o->dt.i<0?0:(ii_o->dt.i>SLL_MAX_INSTRUCTION_INDEX?SLL_MAX_INSTRUCTION_INDEX:(sll_instruction_index_t)(ii_o->dt.i)));
		SLL_RELEASE(ii_o);
	}
	return _ii_to_loc(ii);
}



static sll_object_t* _debug_get_name(const sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* o=SLL_CREATE();
	o->t=SLL_OBJECT_TYPE_STRING;
	if (!all){
		sll_string_create(0,&(o->dt.s));
		return o;
	}
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



void _register_debug_functions(void){
	sll_register_internal_function(sll_current_runtime_data->ift,SLL_CHAR("sll_ext:debug__set_type"),_debug__set_type,0);
	sll_register_internal_function(sll_current_runtime_data->ift,SLL_CHAR("sll_ext:debug_get_call_stack"),_debug_get_call_stack,0);
	sll_register_internal_function(sll_current_runtime_data->ift,SLL_CHAR("sll_ext:debug_get_instruction_count"),_debug_get_instruction_count,0);
	sll_register_internal_function(sll_current_runtime_data->ift,SLL_CHAR("sll_ext:debug_get_instruction_index"),_debug_get_instruction_index,0);
	sll_register_internal_function(sll_current_runtime_data->ift,SLL_CHAR("sll_ext:debug_get_location"),_debug_get_location,0);
	sll_register_internal_function(sll_current_runtime_data->ift,SLL_CHAR("sll_ext:debug_get_name"),_debug_get_name,0);
	sll_register_internal_function(sll_current_runtime_data->ift,SLL_CHAR("sll_ext:debug_get_ref_count"),_debug_get_ref_count,0);
	sll_register_internal_function(sll_current_runtime_data->ift,SLL_CHAR("sll_ext:debug_get_vm_config"),_debug_get_vm_config,0);
}

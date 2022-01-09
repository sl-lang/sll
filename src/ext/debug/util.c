#include <sll.h>



sll_object_type_t debug_cs_type=0;
sll_object_type_t debug_cs_raw_type=0;
sll_object_type_t debug_loc_type=0;
sll_object_type_t debug_vm_cfg_type=0;



sll_object_t* ii_to_loc(sll_instruction_index_t ii){
	if (!debug_loc_type){
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
	sll_object_t* o=sll_create_object_type(sll_current_runtime_data->tt,debug_loc_type,dt,3);
	SLL_RELEASE(fp);
	SLL_RELEASE(fn);
	SLL_RELEASE(dt[2]);
	return o;
}




sll_object_t* debug_init(sll_object_t*const* al,sll_arg_count_t all){
	if (all>3){
		const sll_object_t* a=*al;
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_INT){
			debug_cs_type=(sll_object_type_t)(a->dt.i);
		}
		a=*(al+1);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_INT){
			debug_cs_raw_type=(sll_object_type_t)(a->dt.i);
		}
		a=*(al+2);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_INT){
			debug_loc_type=(sll_object_type_t)(a->dt.i);
		}
		a=*(al+3);
		if (SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_INT){
			debug_vm_cfg_type=(sll_object_type_t)(a->dt.i);
		}
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



sll_object_t* debug_get_ref_count(sll_object_t*const* al,sll_arg_count_t all){
	if (!all){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	return SLL_FROM_INT((*al)->rc);
}

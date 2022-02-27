#include <sll.h>



sll_object_t* ii_to_loc(sll_instruction_index_t ii){
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
	sll_object_t* o=SLL_CREATE();
	o->t=SLL_OBJECT_TYPE_ARRAY;
	sll_array_create(3,&(o->dt.a));
	o->dt.a.v[0]=fp;
	o->dt.a.v[1]=fn;
	o->dt.a.v[2]=SLL_FROM_INT(ln);
	return o;
}



sll_object_t* debug_get_ref_count(sll_object_t*const* al,sll_arg_count_t all){
	if (!all){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	return SLL_FROM_INT((*al)->rc);
}

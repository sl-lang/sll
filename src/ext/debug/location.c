#include <debug/util.h>
#include <sll.h>



sll_object_t* debug_get_location(sll_object_t*const* al,sll_arg_count_t all){
	sll_instruction_index_t ii;
	if (all){
		sll_object_t* ii_o=sll_operator_cast((sll_object_t*)(*al),sll_static_int[SLL_OBJECT_TYPE_INT]);
		ii=(ii_o->dt.i<0?0:(ii_o->dt.i>SLL_MAX_INSTRUCTION_INDEX?SLL_MAX_INSTRUCTION_INDEX:(sll_instruction_index_t)(ii_o->dt.i)));
		SLL_RELEASE(ii_o);
	}
	else{
		const sll_call_stack_t* c_st=sll_get_call_stack();
		ii=(c_st->l?(c_st->dt+c_st->l-1)->_ii:sll_current_instruction_index);
	}
	return ii_to_loc(ii);
}



sll_object_t* debug_get_name(sll_object_t*const* al,sll_arg_count_t all){
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

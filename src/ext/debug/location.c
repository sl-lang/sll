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

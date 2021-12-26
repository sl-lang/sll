#include <debug/util.h>
#include <sll.h>



sll_object_t* debug_get_instruction_count(sll_object_t*const* al,sll_arg_count_t all){
	return SLL_FROM_INT(sll_current_instruction_count);
}



sll_object_t* debug_get_instruction_index(sll_object_t*const* al,sll_arg_count_t all){
	return SLL_FROM_INT(sll_current_instruction_index);
}



sll_object_t* debug_get_vm_config(sll_object_t*const* al,sll_arg_count_t all){
	if (!debug_vm_cfg_type){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	sll_object_t* dt[4]={
		SLL_FROM_INT(sll_current_vm_config->s_sz),
		SLL_FROM_INT(sll_file_to_handle(sll_current_vm_config->in)),
		SLL_FROM_INT(sll_file_to_handle(sll_current_vm_config->out)),
		SLL_FROM_INT(sll_file_to_handle(sll_current_vm_config->err))
	};
	sll_object_t* o=sll_create_object_type(sll_current_runtime_data->tt,debug_vm_cfg_type,dt,4);
	SLL_RELEASE(dt[0]);
	SLL_RELEASE(dt[1]);
	SLL_RELEASE(dt[2]);
	SLL_RELEASE(dt[3]);
	return o;
}

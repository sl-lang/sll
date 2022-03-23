#include <sll/_internal/api.h>
#include <sll/_internal/scheduler.h>
#include <sll/_internal/static_string.h>
#include <sll/api.h>
#include <sll/api/file.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/location.h>
#include <sll/new_object.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/thread.h>
#include <sll/types.h>
#include <sll/vm.h>



static __STATIC_STRING(_vm_code_name,"@code");



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_instruction_to_location(sll_instruction_index_t ii){
	sll_string_index_t fp_i;
	sll_string_index_t fn_i;
	sll_file_offset_t ln=sll_get_location(sll_current_runtime_data->a_dt,ii,&fp_i,&fn_i);
	return sll_new_object(SLL_CHAR("[sis]"),sll_current_runtime_data->a_dt->st.dt+fp_i,ln,(fn_i==SLL_MAX_STRING_INDEX?&_vm_code_name:sll_current_runtime_data->a_dt->st.dt+fn_i));
}



__API_FUNC(vm_get_config){
	sll_new_object_array(SLL_CHAR("iiiii"),out,sll_current_vm_config->s_sz,sll_current_vm_config->c_st_sz,sll_file_to_handle(sll_current_vm_config->in),sll_file_to_handle(sll_current_vm_config->out),sll_file_to_handle(sll_current_vm_config->err));
}



__API_FUNC(vm_get_instruction_count){
	return sll_vm_get_instruction_count();
}



__API_FUNC(vm_get_instruction_index){
	return sll_thread_get_instruction_index(SLL_UNKNOWN_THREAD_INDEX);
}



__API_FUNC(vm_get_location){
	sll_instruction_index_t ii;
	if (a<0){
		const sll_call_stack_t* c_st=sll_thread_get_call_stack(_scheduler_current_thread_index);
		ii=(c_st->l?(c_st->dt+c_st->l-1)->_ii:sll_thread_get_instruction_index(SLL_UNKNOWN_THREAD_INDEX));
	}
	else{
		ii=(a>SLL_MAX_INSTRUCTION_INDEX?SLL_MAX_INSTRUCTION_INDEX:(sll_instruction_index_t)a);
	}
	return sll_instruction_to_location(ii);
}



__API_FUNC(vm_get_ref_count){
	return a->rc;
}

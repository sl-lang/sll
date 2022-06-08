#include <sll/_internal/scheduler.h>
#include <sll/_internal/static_string.h>
#include <sll/api/file.h>
#include <sll/api/vm.h>
#include <sll/array.h>
#include <sll/audit.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/location.h>
#include <sll/new_object.h>
#include <sll/sandbox.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/string_table.h>
#include <sll/thread.h>
#include <sll/types.h>
#include <sll/vm.h>



static __STATIC_STRING(_vm_code_name,"@code");



__SLL_EXTERNAL __SLL_API_CALL void sll_api_vm_get_config(sll_array_t* out){
	sll_new_object_array(SLL_CHAR("iiuuu"),out,sll_current_vm_config->stack_size,sll_current_vm_config->call_stack_size,sll_file_to_handle(sll_current_vm_config->in),sll_file_to_handle(sll_current_vm_config->out),sll_file_to_handle(sll_current_vm_config->err));
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_size_t sll_api_vm_get_instruction_count(void){
	return sll_vm_get_instruction_count();
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_instruction_index_t sll_api_vm_get_instruction_index(void){
	return sll_thread_get_instruction_index(SLL_UNKNOWN_THREAD_INDEX);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_vm_get_location(sll_instruction_index_t instruction_index,sll_array_t* out){
	if (!instruction_index){
		const sll_call_stack_t* c_st=sll_thread_get_call_stack(_scheduler_current_thread_index);
		instruction_index=(c_st->length?(c_st->data+c_st->length-1)->_instruction_index:sll_thread_get_instruction_index(SLL_UNKNOWN_THREAD_INDEX));
	}
	else{
		instruction_index--;
	}
	sll_audit(SLL_CHAR("sll.vm.location"),SLL_CHAR("h"),instruction_index);
	sll_object_t* o=sll_instruction_to_location(instruction_index);
	*out=o->data.array;
	SLL_CRITICAL(sll_destroy_object(o));
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_reference_count_t sll_api_vm_get_ref_count(sll_object_t* object){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_REFERENCE_COUNTER)){
		return 0;
	}
	sll_audit(SLL_CHAR("sll.vm.ref"),SLL_CHAR("O"),object);
	return object->rc;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_instruction_to_location(sll_instruction_index_t instruction_index){
	sll_string_index_t fp_i;
	sll_string_index_t fn_i;
	sll_file_offset_t ln=sll_get_location(sll_current_runtime_data->assembly_data,instruction_index,&fp_i,&fn_i);
	return sll_new_object(SLL_CHAR("(sis)"),sll_current_runtime_data->assembly_data->string_table.data+fp_i,ln,(fn_i==SLL_MAX_STRING_INDEX?&_vm_code_name:sll_current_runtime_data->assembly_data->string_table.data+fn_i));
}

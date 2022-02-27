#include <debug/call_stack.h>
#include <debug/location.h>
#include <debug/util.h>
#include <debug/vm.h>
#include <sll.h>




#ifdef _MSC_VER
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __attribute__((visibility("default")))
#endif



EXPORT sll_module_loader_data_t __sll_module=__SLL_EXT_MODULE_DATA;



EXPORT sll_bool_t __sll_load(sll_version_t v){
	if (v!=SLL_VERSION){
		SLL_LOG("Version mismatch!");
		return 0;
	}
	sll_register_internal_function(sll_current_runtime_data->ift,SLL_CHAR("sll_ext_debug:get_call_stack"),debug_get_call_stack);
	sll_register_internal_function(sll_current_runtime_data->ift,SLL_CHAR("sll_ext_debug:get_instruction_count"),debug_get_instruction_count);
	sll_register_internal_function(sll_current_runtime_data->ift,SLL_CHAR("sll_ext_debug:get_instruction_index"),debug_get_instruction_index);
	sll_register_internal_function(sll_current_runtime_data->ift,SLL_CHAR("sll_ext_debug:get_location"),debug_get_location);
	sll_register_internal_function(sll_current_runtime_data->ift,SLL_CHAR("sll_ext_debug:get_name"),debug_get_name);
	sll_register_internal_function(sll_current_runtime_data->ift,SLL_CHAR("sll_ext_debug:get_ref_count"),debug_get_ref_count);
	sll_register_internal_function(sll_current_runtime_data->ift,SLL_CHAR("sll_ext_debug:get_vm_config"),debug_get_vm_config);
	return 1;
}



EXPORT void __sll_unload(void){
}

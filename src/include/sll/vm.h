#ifndef __SLL_VM_H__
#define __SLL_VM_H__ 1
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name VM
 * \group vm
 * \desc Docs!
 */



#define SLL_CALL_STACK_SIZE 256



/**
 * \flags var
 * \name sll_current_instruction_count
 * \group assembly
 * \desc Docs!
 * \type sll_integer_t
 */
__SLL_EXTERNAL extern sll_integer_t sll_current_instruction_count;



/**
 * \flags var
 * \name sll_current_runtime_data
 * \group assembly
 * \desc Docs!
 * \type const sll_runtime_data_t*
 */
__SLL_EXTERNAL extern const sll_runtime_data_t* sll_current_runtime_data;



/**
 * \flags var
 * \name sll_current_vm_config
 * \group assembly
 * \desc Docs!
 * \type const sll_vm_config_t*
 */
__SLL_EXTERNAL extern const sll_vm_config_t* sll_current_vm_config;



/**
 * \flags check_output func
 * \name sll_execute_assembly
 * \group vm
 * \desc Docs!
 * \arg const sll_assembly_data_t* a_dt
 * \arg const sll_vm_config_t* cfg
 * \arg sll_error_t* e
 * \ret sll_return_code_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_return_code_t sll_execute_assembly(const sll_assembly_data_t* a_dt,const sll_vm_config_t* cfg,sll_error_t* e);



#endif

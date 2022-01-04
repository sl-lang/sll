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



#define SLL_VM_INVALID_INSTRUCTION_INDEX (-0x7fffffff)
#define SLL_VM_INVALID_STACK_INDEX (-0x80000000ll)



/**
 * \flags var
 * \name sll_current_instruction_count
 * \group assembly
 * \desc Docs!
 * \type sll_instruction_index_t
 */
__SLL_EXTERNAL extern sll_instruction_index_t sll_current_instruction_count;



/**
 * \flags var
 * \name sll_current_instruction_index
 * \group assembly
 * \desc Docs!
 * \type sll_instruction_index_t
 */
__SLL_EXTERNAL extern sll_instruction_index_t sll_current_instruction_index;



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
 * \ret sll_return_code_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_return_code_t sll_execute_assembly(const sll_assembly_data_t* a_dt,const sll_vm_config_t* cfg);



#endif

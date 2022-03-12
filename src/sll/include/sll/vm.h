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



/**
 * \flags var
 * \name sll_current_runtime_data
 * \group vm
 * \desc Docs!
 * \type const sll_runtime_data_t*
 */
__SLL_EXTERNAL extern const sll_runtime_data_t* sll_current_runtime_data;



/**
 * \flags var
 * \name sll_current_vm_config
 * \group vm
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



/**
 * \flags check_output func
 * \name sll_execute_function
 * \group vm
 * \desc Docs!
 * \arg sll_integer_t fn_idx
 * \arg sll_object_t*const* al
 * \arg sll_arg_count_t all
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_execute_function(sll_integer_t fn_idx,sll_object_t*const* al,sll_arg_count_t all);



/**
 * \flags check_output func
 * \name sll_vm_get_instruction_count
 * \group vm
 * \desc Docs!
 * \ret sll_size_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_size_t sll_vm_get_instruction_count(void);



/**
 * \flags check_output func
 * \name sll_wait_thread
 * \group vm
 * \desc Docs!
 * \arg sll_thread_index_t tid
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_wait_thread(sll_thread_index_t tid);



#endif

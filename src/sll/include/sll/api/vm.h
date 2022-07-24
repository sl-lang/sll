#ifndef __SLL_API_VM_H__
#define __SLL_API_VM_H__ 1
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/types.h>
/**
 * \flags group
 * \name VM API
 * \group vm-api
 * \desc Docs!
 */



/**
 * \flags func
 * \name sll_api_vm_get_config
 * \group vm-api
 * \desc Docs!
 * \api |a
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_vm_get_config(sll_array_t* out);



/**
 * \flags check_output func
 * \name sll_api_vm_get_instruction_count
 * \group vm-api
 * \desc Docs!
 * \api |Q
 * \ret sll_size_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_size_t sll_api_vm_get_instruction_count(void);



/**
 * \flags check_output func
 * \name sll_api_vm_get_instruction_index
 * \group vm-api
 * \desc Docs!
 * \api |D
 * \ret sll_instruction_index_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_instruction_index_t sll_api_vm_get_instruction_index(void);



/**
 * \flags func
 * \name sll_api_vm_get_location
 * \group vm-api
 * \desc Docs!
 * \api D|a
 * \arg sll_instruction_index_t instruction_index
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_vm_get_location(sll_instruction_index_t instruction_index,sll_array_t* out);



/**
 * \flags check_output func
 * \name sll_api_vm_get_ref_count
 * \group vm-api
 * \desc Docs!
 * \api o|D
 * \arg sll_object_t object
 * \ret sll_reference_count_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_reference_count_t sll_api_vm_get_ref_count(sll_object_t object);



/**
 * \flags check_output func
 * \name sll_instruction_to_location
 * \group vm-api
 * \desc Docs!
 * \arg sll_instruction_index_t instruction_index
 * \ret sll_object_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t sll_instruction_to_location(sll_instruction_index_t instruction_index);



#endif

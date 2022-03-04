#ifndef __SLL_API_VM_H__
#define __SLL_API_VM_H__ 1
#include <sll/api.h>
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name VM API
 * \group vm-api
 * \desc Docs!
 */



/**
 * \flags check_output func
 * \name sll_instruction_to_location
 * \group vm-api
 * \desc Docs!
 * \arg sll_instruction_index_t ii
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_instruction_to_location(sll_instruction_index_t ii);



/**
 * \flags api func
 * \name sll_api_vm_get_config
 * \group vm-api
 * \desc Docs!
 * \ret A
 */
__API_FUNC_DECL(vm_get_config);



/**
 * \flags api func
 * \name sll_api_vm_get_instruction_count
 * \group vm-api
 * \desc Docs!
 * \ret I
 */
__API_FUNC_DECL(vm_get_instruction_count);



/**
 * \flags api func
 * \name sll_api_vm_get_instruction_index
 * \group vm-api
 * \desc Docs!
 * \ret I
 */
__API_FUNC_DECL(vm_get_instruction_index);



/**
 * \flags api func
 * \name sll_api_vm_get_location
 * \group vm-api
 * \desc Docs!
 * \arg I
 * \ret O
 */
__API_FUNC_DECL(vm_get_location);



/**
 * \flags api func
 * \name sll_api_vm_get_ref_count
 * \group vm-api
 * \desc Docs!
 * \arg O
 * \ret I
 */
__API_FUNC_DECL(vm_get_ref_count);



#endif

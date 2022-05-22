#ifndef __SLL_VM_H__
#define __SLL_VM_H__ 1
#include <sll/_size_types.h>
#include <sll/_thread_type.h>
#include <sll/assembly.h>
#include <sll/common.h>
#include <sll/file.h>
#include <sll/gc.h>
#include <sll/ift.h>
#include <sll/object.h>
#include <sll/platform/process.h>
#include <sll/types.h>
/**
 * \flags group
 * \name VM
 * \group vm
 * \desc Docs!
 */



/**
 * \flags macro var
 * \name SLL_EXECUTE_FUNCTION_ASYNC
 * \group vm
 * \desc Docs!
 * \type sll_execution_flags_t
 */
#define SLL_EXECUTE_FUNCTION_ASYNC 1



/**
 * \flags macro var
 * \name SLL_EXECUTE_FUNCTION_RESERVED0
 * \group vm
 * \desc Internally used as `EXECUTE_FUNCTION_NO_AUDIT_TERMINATE`
 * \type sll_execution_flags_t
 */
#define SLL_EXECUTE_FUNCTION_RESERVED0 2



/**
 * \flags type var
 * \name sll_execution_flags_t
 * \group vm
 * \desc Docs!
 * \type __SLL_U8
 */
typedef __SLL_U8 sll_execution_flags_t;



/**
 * \flags type var
 * \name sll_call_stack_size_t
 * \group vm
 * \desc Docs!
 * \type __SLL_U16
 */
typedef __SLL_U16 sll_call_stack_size_t;



/**
 * \flags type
 * \name sll_call_stack_frame_t
 * \group vm
 * \desc Docs!
 * \arg const sll_char_t* name
 * \arg sll_instruction_index_t _instruction_index
 * \arg sll_stack_offset_t _stack_offset
 * \arg void* _variable_memory_offset
 */
typedef struct _SLL_CALL_STACK_FRAME{
	const sll_char_t* name;
	sll_instruction_index_t _instruction_index;
	sll_stack_offset_t _stack_offset;
	void* _variable_memory_offset;
} sll_call_stack_frame_t;



/**
 * \flags type
 * \name sll_call_stack_t
 * \group vm
 * \desc Docs!
 * \arg sll_call_stack_frame_t* data
 * \arg sll_call_stack_size_t length
 */
typedef struct _SLL_CALL_STACK{
	sll_call_stack_frame_t* data;
	sll_call_stack_size_t length;
} sll_call_stack_t;



/**
 * \flags type
 * \name sll_runtime_data_t
 * \group vm
 * \desc Docs!
 * \arg const sll_assembly_data_t* assembly_data
 * \arg sll_internal_function_table_t* internal_function_table
 * \arg sll_object_type_table_t* type_table
 */
typedef struct _SLL_RUNTIME_DATA{
	const sll_assembly_data_t* assembly_data;
	sll_internal_function_table_t* internal_function_table;
	sll_object_type_table_t* type_table;
} sll_runtime_data_t;



/**
 * \flags type
 * \name sll_vm_config_t
 * \group vm
 * \desc Docs!
 * \arg sll_size_t s_sz
 * \arg sll_size_t call_stack_size
 * \arg sll_internal_function_table_t* internal_function_table
 * \arg sll_file_t* in
 * \arg sll_file_t* out
 * \arg sll_file_t* err
 */
typedef struct _SLL_VM_CONFIG{
	sll_size_t stack_size;
	sll_size_t call_stack_size;
	sll_internal_function_table_t* internal_function_table;
	sll_file_t* in;
	sll_file_t* out;
	sll_file_t* err;
} sll_vm_config_t;



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
 * \arg sll_execution_flags_t fl
 * \arg sll_object_t* out
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_execute_function(sll_integer_t fn_idx,sll_object_t*const* al,sll_arg_count_t all,sll_execution_flags_t fl);



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
 * \arg sll_object_t* out
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_wait_thread(sll_thread_index_t tid);



#endif

#ifndef __SLL_PLATFORM_PROCESS_H__
#define __SLL_PLATFORM_PROCESS_H__ 1
#include <sll/common.h>
#include <sll/error.h>
#include <sll/types.h>
/**
 * \flags subgroup
 * \name Processes
 * \group platform
 * \subgroup platform-process
 * \desc Docs!
 */



/**
 * \flags type var
 * \name sll_pid_t
 * \group platform
 * \subgroup platform-process
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_pid_t;



/**
 * \flags type var
 * \name sll_process_handle_t
 * \group platform
 * \subgroup platform-process
 * \desc Docs!
 * \type void*
 */
typedef void* sll_process_handle_t;



/**
 * \flags check_output func
 * \name sll_platform_close_process_handle
 * \group platform
 * \subgroup platform-process
 * \desc Docs!
 * \arg sll_process_handle_t ph
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_close_process_handle(sll_process_handle_t ph);



/**
 * \flags check_output func
 * \name sll_platform_execute_shell
 * \group platform
 * \subgroup platform-process
 * \desc Docs!
 * \arg const sll_char_t* cmd
 * \arg sll_error_t* err
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_execute_shell(const sll_char_t* cmd,sll_error_t* err);



/**
 * \flags check_output func
 * \name sll_platform_get_pid
 * \group platform
 * \subgroup platform-process
 * \desc Docs!
 * \ret sll_pid_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_pid_t sll_platform_get_pid(void);



/**
 * \flags check_output func
 * \name sll_platform_start_process
 * \group platform
 * \subgroup platform-process
 * \desc Docs!
 * \arg const sll_char_t*const* a
 * \arg const sll_char_t*const* env
 * \arg sll_error_t* err
 * \ret sll_process_handle_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_process_handle_t sll_platform_start_process(const sll_char_t*const* a,const sll_char_t*const* env,sll_error_t* err);



/**
 * \flags check_output func
 * \name sll_platform_wait_process
 * \group platform
 * \subgroup platform-process
 * \desc Docs!
 * \arg sll_process_handle_t ph
 * \arg sll_error_t* err
 * \ret sll_return_code_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_return_code_t sll_platform_wait_process(sll_process_handle_t ph,sll_error_t* err);



#endif

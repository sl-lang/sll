#ifndef __SLL_PLATFORM_PROCESS_H__
#define __SLL_PLATFORM_PROCESS_H__ 1
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags subgroup
 * \name Processes
 * \group platform
 * \subgroup platform-process
 * \desc Docs!
 */



/**
 * \flags func
 * \name sll_platform_close_process_handle
 * \group platform
 * \subgroup platform-process
 * \desc Docs!
 * \arg sll_process_handle_t ph
 */
__SLL_EXTERNAL void sll_platform_close_process_handle(sll_process_handle_t ph);



/**
 * \flags check_output func
 * \name sll_platform_execute_shell
 * \group platform
 * \subgroup platform-process
 * \desc Docs!
 * \arg const sll_char_t* cmd
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_execute_shell(const sll_char_t* cmd);



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
 * \ret sll_process_handle_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_process_handle_t sll_platform_start_process(const sll_char_t*const* a,const sll_char_t*const* env);



/**
 * \flags check_output func
 * \name sll_platform_wait_process
 * \group platform
 * \subgroup platform-process
 * \desc Docs!
 * \arg sll_process_handle_t ph
 * \ret sll_return_code_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_return_code_t sll_platform_wait_process(sll_process_handle_t ph);



#endif

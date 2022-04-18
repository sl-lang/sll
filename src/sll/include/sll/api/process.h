#ifndef __SLL_API_PROCESS_H__
#define __SLL_API_PROCESS_H__ 1
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Process API
 * \group process-api
 * \desc Docs!
 */
/**
 * \flags subgroup
 * \name Flags
 * \group process-api
 * \subgroup process-api-flag
 * \desc Docs!
 */



/**
 * \flags macro var
 * \name SLL_PROCESS_FLAG_PASS_STDIN
 * \group process-api
 * \subgroup process-api-flag
 * \desc Docs!
 * \type sll_flags_t
 */
#define SLL_PROCESS_FLAG_PASS_STDIN 1



/**
 * \flags macro var
 * \name SLL_PROCESS_FLAG_CAPTURE_STDOUT
 * \group process-api
 * \subgroup process-api-flag
 * \desc Docs!
 * \type sll_flags_t
 */
#define SLL_PROCESS_FLAG_CAPTURE_STDOUT 2



/**
 * \flags macro var
 * \name SLL_PROCESS_FLAG_CAPTURE_STDERR
 * \group process-api
 * \subgroup process-api-flag
 * \desc Docs!
 * \type sll_flags_t
 */
#define SLL_PROCESS_FLAG_CAPTURE_STDERR 4



/**
 * \flags macro var
 * \name SLL_PROCESS_FLAG_WAIT
 * \group process-api
 * \subgroup process-api-flag
 * \desc Docs!
 * \type sll_flags_t
 */
#define SLL_PROCESS_FLAG_WAIT 8



/**
 * \flags func
 * \name sll_process_join_args
 * \group process-api
 * \desc Docs!
 * \arg const sll_char_t*const* a
 * \arg sll_string_t* o
 */
__SLL_EXTERNAL void sll_process_join_args(const sll_char_t*const* a,sll_string_t* o);



/**
 * \flags func
 * \name sll_api_process_execute_shell
 * \group process-api
 * \desc Docs!
 * \api s|i
 * \arg sll_string_t* cmd
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_process_execute_shell(sll_string_t* cmd);



/**
 * \flags func
 * \name sll_api_process_get_pid
 * \group process-api
 * \desc Docs!
 * \api |i
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_process_get_pid(void);



/**
 * \flags func
 * \name sll_api_process_join
 * \group process-api
 * \desc Docs!
 * \api s+|s
 * \arg sll_string_t*const* args
 * \arg sll_arg_count_t len
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_process_join(sll_string_t*const* args,sll_arg_count_t len,sll_string_t* out);



/**
 * \flags func
 * \name sll_api_process_split
 * \group process-api
 * \desc Docs!
 * \api s|a
 * \arg sll_string_t* args
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_process_split(sll_string_t* args,sll_array_t* out);



/**
 * \flags func
 * \name sll_api_process_start
 * \group process-api
 * \desc Docs!
 * \api asis|a
 * \arg sll_array_t* args
 * \arg sll_string_t* cwd
 * \arg sll_integer_t flags
 * \arg sll_string_t* stdin
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_process_start(sll_array_t* args,sll_string_t* cwd,sll_integer_t flags,sll_string_t* stdin,sll_array_t* out);



#endif

#ifndef __SLL_API_PROCESS_H__
#define __SLL_API_PROCESS_H__ 1
#include <sll/api.h>
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Process API
 * \group process-api
 * \desc Docs!
 */



#define SLL_PROCESS_FLAG_PASS_STDIN 1
#define SLL_PROCESS_FLAG_CAPTURE_STDOUT 2
#define SLL_PROCESS_FLAG_CAPTURE_STDERR 4
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
 * \flags api func
 * \name sll_api_process__init
 * \group process-api
 * \desc Docs!
 * \arg I
 */
__API_FUNC_DECL(process__init);



/**
 * \flags api func
 * \name sll_api_process_join
 * \group process-api
 * \desc Docs!
 * \arg A
 * \ret S
 */
__API_FUNC_DECL(process_join);



/**
 * \flags api func
 * \name sll_api_process_execute_shell
 * \group process-api
 * \desc Docs!
 * \arg S
 * \ret B
 */
__API_FUNC_DECL(process_execute_shell);



/**
 * \flags api func optimizable
 * \name sll_api_process_get_pid
 * \group process-api
 * \desc Docs!
 * \ret I
 */
__API_FUNC_DECL(process_get_pid);



/**
 * \flags api func
 * \name sll_api_process_start
 * \group process-api
 * \desc Docs!
 * \arg A
 * \arg O
 * \arg S
 * \ret O
 */
__API_FUNC_DECL(process_start);



/**
 * \flags api func
 * \name sll_api_process_split
 * \group process-api
 * \desc Docs!
 * \arg S
 * \ret A
 */
__API_FUNC_DECL(process_split);



#endif

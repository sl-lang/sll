#ifndef __SLL_THREAD_H__
#define __SLL_THREAD_H__ 1
#include <sll/_size_types.h>
#include <sll/_thread_type.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/types.h>
#include <sll/vm.h>
/**
 * \flags group
 * \name Threads
 * \group thread
 * \desc Docs!
 */



/**
 * \flags macro var
 * \name SLL_MAX_THREAD_INDEX
 * \group thread
 * \desc Docs!
 * \type sll_thread_index_t
 */
#define SLL_MAX_THREAD_INDEX 0xfffffffe



/**
 * \flags macro var
 * \name SLL_UNKNOWN_THREAD_INDEX
 * \group thread
 * \desc Docs!
 * \type sll_thread_index_t
 */
#define SLL_UNKNOWN_THREAD_INDEX 0xffffffff



/**
 * \flags check_output func
 * \name sll_thread_create
 * \group thread
 * \desc Docs!
 * \arg sll_integer_t fn
 * \arg sll_object_t*const* al
 * \arg sll_arg_count_t all
 * \ret sll_thread_index_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_thread_index_t sll_thread_create(sll_integer_t fn,sll_object_t*const* al,sll_arg_count_t all);



/**
 * \flags check_output func
 * \name sll_thread_delete
 * \group thread
 * \desc Docs!
 * \arg sll_thread_index_t t
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_thread_delete(sll_thread_index_t t);



/**
 * \flags check_output func
 * \name sll_thread_get_call_stack
 * \group thread
 * \desc Docs!
 * \arg sll_thread_index_t t
 * \ret const sll_call_stack_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT const sll_call_stack_t* sll_thread_get_call_stack(sll_thread_index_t t);



/**
 * \flags check_output func
 * \name sll_thread_get_instruction_index
 * \group thread
 * \desc Docs!
 * \arg sll_thread_index_t t
 * \ret sll_instruction_index_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_instruction_index_t sll_thread_get_instruction_index(sll_thread_index_t t);



/**
 * \flags check_output func
 * \name sll_thread_restart
 * \group thread
 * \desc Docs!
 * \arg sll_thread_index_t t
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_thread_restart(sll_thread_index_t t);



/**
 * \flags check_output func
 * \name sll_thread_start
 * \group thread
 * \desc Docs!
 * \arg sll_thread_index_t t
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_thread_start(sll_thread_index_t t);



/**
 * \flags check_output func
 * \name sll_thread_suspend
 * \group thread
 * \desc Docs!
 * \arg sll_thread_index_t t
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_thread_suspend(sll_thread_index_t t);



#endif

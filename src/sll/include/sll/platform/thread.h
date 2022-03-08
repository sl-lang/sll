#ifndef __SLL_PLATFORM_THREAD_H__
#define __SLL_PLATFORM_THREAD_H__ 1
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags subgroup
 * \name Threads
 * \group platform
 * \subgroup platform-thread
 * \desc Docs!
 */



/**
 * \flags macro var
 * \name SLL_UNKNOWN_INTERNAL_THREAD_INDEX
 * \group platform
 * \subgroup platform-thread
 * \desc Docs!
 * \type sll_internal_thread_index_t
 */
#define SLL_UNKNOWN_INTERNAL_THREAD_INDEX ((void*)0xffffffffffffffffull)



/**
 * \flags check_output func
 * \name sll_platform_join_thread
 * \group platform
 * \subgroup platform-thread
 * \desc Docs!
 * \arg sll_internal_thread_index_t tid
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_join_thread(sll_internal_thread_index_t tid);



/**
 * \flags check_output func
 * \name sll_platform_start_thread
 * \group platform
 * \subgroup platform-thread
 * \desc Docs!
 * \arg sll_internal_thread_function_t fn
 * \arg void* arg
 * \ret sll_internal_thread_index_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_internal_thread_index_t sll_platform_start_thread(sll_internal_thread_function_t fn,void* arg);



#endif

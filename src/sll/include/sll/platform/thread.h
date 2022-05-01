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
 * \name SLL_CPU_ANY
 * \group platform
 * \subgroup platform-thread
 * \desc Docs!
 * \type sll_cpu_t
 */
#define SLL_CPU_ANY 0xffff



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
 * \flags type var
 * \name sll_cpu_t
 * \group platform
 * \subgroup platform-thread
 * \desc Docs!
 * \type __SLL_U16
 */
typedef __SLL_U16 sll_cpu_t;



/**
 * \flags type var
 * \name sll_internal_thread_index_t
 * \group platform
 * \subgroup platform-thread
 * \desc Docs!
 * \type void*
 */
typedef void* sll_internal_thread_index_t;



/**
 * \flags func type
 * \name sll_internal_thread_function_t
 * \group platform
 * \subgroup platform-thread
 * \desc Docs!
 * \arg void* arg
 */
typedef void (*sll_internal_thread_function_t)(void* arg);



/**
 * \flags check_output func
 * \name sll_platform_current_thread
 * \group platform
 * \subgroup platform-thread
 * \desc Docs!
 * \ret sll_internal_thread_index_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_internal_thread_index_t sll_platform_current_thread(void);



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
 * \name sll_platform_set_cpu
 * \group platform
 * \subgroup platform-thread
 * \desc Docs!
 * \arg sll_cpu_t cpu
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_set_cpu(sll_cpu_t cpu);



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

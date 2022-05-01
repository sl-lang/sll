#ifndef __SLL_SCHEDULER_H__
#define __SLL_SCHEDULER_H__ 1
#include <sll/common.h>
#include <sll/platform/thread.h>
#include <sll/thread.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Scheduler
 * \group scheduler
 * \desc Docs!
 */



/**
 * \flags check_output func
 * \name sll_get_cpu_index
 * \group scheduler
 * \desc Docs!
 * \ret sll_cpu_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_cpu_t sll_get_cpu_index(void);



/**
 * \flags check_output func
 * \name sll_get_thread_index
 * \group scheduler
 * \desc Docs!
 * \ret sll_thread_index_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_thread_index_t sll_get_thread_index(void);



#endif

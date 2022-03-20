#ifndef __SLL_SCHEDULER_H__
#define __SLL_SCHEDULER_H__ 1
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Scheduler
 * \group scheduler
 * \desc Docs!
 */



/**
 * \flags macro var
 * \name SLL_SCHEDULER_MAX_THREADS
 * \group scheduler
 * \desc Docs!
 * \type sll_thread_index_t
 */
#define SLL_SCHEDULER_MAX_THREADS 16384



/**
 * \flags check_output func
 * \name sll_get_thread_index
 * \group scheduler
 * \desc Docs!
 * \ret sll_thread_index_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_thread_index_t sll_get_thread_index(void);



#endif

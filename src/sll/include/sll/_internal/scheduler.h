#ifndef __SLL__INTERNAL_SCHEDULER_H__
#define __SLL__INTERNAL_SCHEDULER_H__ 1
#include <sll/_internal/common.h>
#include <sll/_internal/thread.h>
#include <sll/_size_types.h>
#include <sll/platform/event.h>
#include <sll/platform/lock.h>
#include <sll/platform/process.h>
#include <sll/platform/thread.h>
#include <sll/thread.h>
#include <sll/types.h>



#define SCHEDULER_MAX_CPU_CORE_THREADS 16384



typedef __SLL_U32 queue_length_t;



typedef struct _SCHEDULER_CPU_DATA{
	sll_thread_index_t queue[SCHEDULER_MAX_CPU_CORE_THREADS];
	queue_length_t queue_idx;
	queue_length_t queue_len;
	sll_internal_thread_index_t internal_thread_index;
	sll_event_handle_t signal_event;
	sll_lock_handle_t lock;
	sll_cpu_t cpu_id;
	sll_bool_t wait;
} scheduler_cpu_data_t;



typedef struct _LOAD_BALANCER{
	sll_lock_handle_t lock;
	sll_cpu_t length;
	sll_cpu_t index;
} load_balancer_t;



extern __SLL_TLS sll_cpu_t _scheduler_internal_thread_index;
extern __SLL_TLS sll_thread_index_t _scheduler_current_thread_index;
extern __SLL_TLS thread_data_t* _scheduler_current_thread;



void _scheduler_queue_next(void);



sll_thread_index_t _scheduler_queue_pop(void);



sll_return_code_t _scheduler_run(void);



void _scheduler_queue_thread(sll_thread_index_t t);



void _scheduler_set_thread(sll_thread_index_t t);



#endif

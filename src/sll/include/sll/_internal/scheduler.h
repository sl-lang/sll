#ifndef __SLL_INTERNAL_SCHEDULER_H__
#define __SLL_INTERNAL_SCHEDULER_H__ 1
#include <sll/_internal/common.h>
#include <sll/_internal/thread.h>
#include <sll/_size_types.h>
#include <sll/types.h>



typedef __SLL_U32 queue_length_t;



typedef struct __SCHEDULER_CPU_DATA{
	queue_length_t queue_idx;
	queue_length_t queue_len;
	sll_internal_thread_index_t tid;
	sll_event_handle_t sig;
	sll_cpu_t id;
	sll_thread_index_t queue[];
} scheduler_cpu_data_t;



extern __SLL_TLS sll_thread_index_t _scheduler_current_thread_index;
extern __SLL_TLS thread_data_t* _scheduler_current_thread;



void _scheduler_queue_next(void);



sll_thread_index_t _scheduler_queue_pop(void);



sll_return_code_t _scheduler_run(void);



void _scheduler_queue_thread(sll_thread_index_t t);



void _scheduler_set_thread(sll_thread_index_t t);



#endif

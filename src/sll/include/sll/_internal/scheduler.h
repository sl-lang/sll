#ifndef __SLL_INTERNAL_SCHEDULER_H__
#define __SLL_INTERNAL_SCHEDULER_H__ 1
#include <sll/_internal/thread.h>
#include <sll/_size_types.h>
#include <sll/types.h>



typedef __SLL_U32 queue_length_t;



typedef struct __SCHEDULER_CPU_DATA{
	sll_thread_index_t* queue;
	queue_length_t queue_idx;
	queue_length_t queue_len;
	sll_internal_thread_index_t tid;
} scheduler_cpu_data_t;



extern thread_data_t* _scheduler_current_thread;



void _scheduler_deinit(void);



void _scheduler_init(void);



void _scheduler_queue_next(void);



sll_thread_index_t _scheduler_queue_pop(void);



void _scheduler_queue_thread(sll_thread_index_t t);



void _scheduler_set_thread(sll_thread_index_t t);



#endif

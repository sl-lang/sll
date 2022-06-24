#ifndef __SLL__INTERNAL_SEMAPHORE_H__
#define __SLL__INTERNAL_SEMAPHORE_H__ 1
#include <sll/_internal/common.h>
#include <sll/_internal/thread.h>
#include <sll/semaphore.h>
#include <sll/thread.h>
#include <sll/types.h>



typedef struct _SEMAPHORE{
	sll_semaphore_counter_t count;
	sll_thread_index_t first;
	thread_data_t* last;
} semaphore_t;



void _semaphore_deinit(void);



void _semaphore_init(void);



sll_bool_t _semaphore_wait(sll_integer_t sempahore_index);



#endif

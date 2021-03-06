#ifndef __SLL__INTERNAL_BARRIER_H__
#define __SLL__INTERNAL_BARRIER_H__ 1
#include <sll/_internal/common.h>
#include <sll/_internal/thread.h>
#include <sll/barrier.h>
#include <sll/thread.h>
#include <sll/types.h>



typedef struct _BARRIER{
	sll_barrier_counter_t count;
	sll_thread_index_t first;
	thread_data_t* last;
} barrier_t;



void _barrier_deinit(void);



void _barrier_init(void);



sll_bool_t _barrier_wait(sll_integer_t barrier_index,sll_integer_t value,sll_bool_t greate_or_equal);



#endif

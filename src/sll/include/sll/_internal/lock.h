#ifndef __SLL__INTERNAL_LOCK_H__
#define __SLL__INTERNAL_LOCK_H__ 1
#include <sll/_internal/common.h>
#include <sll/_internal/thread.h>
#include <sll/lock.h>
#include <sll/thread.h>
#include <sll/types.h>



typedef struct _LOCK{
	sll_thread_index_t lock;
	sll_thread_index_t first;
	thread_data_t* last;
} lock_t;



sll_bool_t _lock_wait(sll_integer_t lock_index);



void _lock_deinit(void);



void _lock_init(void);



#endif

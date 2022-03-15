#ifndef __SLL_INTERNAL_SEMAPHORE_H__
#define __SLL_INTERNAL_SEMAPHORE_H__ 1
#include <sll/_internal/common.h>
#include <sll/_internal/thread.h>
#include <sll/_size_types.h>
#include <sll/types.h>



#define SEMAPHORE_UNUSED 0xffffffff
#define SEMAPHORE_GET_NEXT_ID(s) ((sll_semaphore_index_t)ADDR((s)->last))
#define SEMAPHORE_SET_NEXT_ID(s,v) ((s)->last=PTR((sll_semaphore_index_t)(v)))



typedef __SLL_U32 semaphore_list_length_t;



typedef struct __SEMAPHORE{
	sll_semaphore_counter_t count;
	sll_thread_index_t first;
	thread_data_t* last;
} semaphore_t;



void _semaphore_deinit(void);



void _semaphore_init(void);



sll_bool_t _semaphore_wait(sll_integer_t w);



#endif

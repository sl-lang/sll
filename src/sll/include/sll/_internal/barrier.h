#ifndef __SLL__INTERNAL_BARRIER_H__
#define __SLL__INTERNAL_BARRIER_H__ 1
#include <sll/_internal/common.h>
#include <sll/_internal/thread.h>
#include <sll/_size_types.h>
#include <sll/barrier.h>
#include <sll/thread.h>
#include <sll/types.h>



#define BARRIER_UNUSED 0xffffffff
#define BARRIER_GET_NEXT_ID(s) ((sll_barrier_index_t)ADDR((s)->last))
#define BARRIER_SET_NEXT_ID(s,v) ((s)->last=PTR((sll_barrier_index_t)(v)))



typedef __SLL_U32 barrier_list_length_t;



typedef struct _BARRIER{
	sll_barrier_counter_t count;
	sll_thread_index_t first;
	thread_data_t* last;
} barrier_t;



void _barrier_deinit(void);



void _barrier_init(void);



sll_bool_t _barrier_wait(sll_integer_t w,sll_integer_t v,sll_bool_t g);



#endif

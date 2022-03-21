#ifndef __SLL__INTERNAL_LOCK_H__
#define __SLL__INTERNAL_LOCK_H__ 1
#include <sll/_internal/common.h>
#include <sll/_internal/thread.h>
#include <sll/_size_types.h>
#include <sll/types.h>



#define LOCK_UNUSED 0xfffffffe
#define LOCK_GET_NEXT_ID(l) ((sll_lock_index_t)ADDR((l)->last))
#define LOCK_SET_NEXT_ID(l,v) ((l)->last=PTR((sll_lock_index_t)(v)))



typedef __SLL_U32 lock_list_length_t;



typedef struct __LOCK{
	sll_thread_index_t lock;
	sll_thread_index_t first;
	thread_data_t* last;
} lock_t;



sll_bool_t _lock_wait(sll_integer_t w);



void _lock_deinit(void);



void _lock_init(void);



#endif

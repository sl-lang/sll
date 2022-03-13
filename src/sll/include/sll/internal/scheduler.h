#ifndef __SLL_INTERNAL_SCHEDULER_H__
#define __SLL_INTERNAL_SCHEDULER_H__ 1
#include <sll/_size_types.h>
#include <sll/internal/common.h>
#include <sll/types.h>



#define THREAD_SCHEDULER_INSTRUCTION_COUNT 10
#define THREAD_IS_UNUSED(t) (ADDR(t)>>63)
#define THREAD_NEXT_UNUSED(id) (PTR((id)|0x8000000000000000ull))
#define THREAD_GET_NEXT_UNUSED(t) ((sll_thread_index_t)(ADDR((t))&0x7fffffffffffffffull))
#define THREAD_SIZE SLL_ROUND_PAGE(sizeof(thread_data_t)+sll_current_vm_config->c_st_sz*sizeof(sll_call_stack_frame_t)+sll_current_vm_config->s_sz*sizeof(sll_object_t*))

#define THREAD_ALLOCATOR_CACHE_POOL_SIZE 16

#define THREAD_BARRIER_UNUSED 0xffffffff
#define THREAD_BARRIER_GET_NEXT_ID(s) ((sll_barrier_index_t)ADDR((s)->last))
#define THREAD_BARRIER_SET_NEXT_ID(s,v) ((s)->last=PTR((sll_barrier_index_t)(v)))

#define THREAD_LOCK_UNUSED 0xfffffffe
#define THREAD_LOCK_GET_NEXT_ID(l) ((sll_lock_index_t)ADDR((l)->last))
#define THREAD_LOCK_SET_NEXT_ID(l,v) ((l)->last=PTR((sll_lock_index_t)(v)))

#define THREAD_SEMAPHORE_UNUSED 0xffffffff
#define THREAD_SEMAPHORE_GET_NEXT_ID(s) ((sll_semaphore_index_t)ADDR((s)->last))
#define THREAD_SEMAPHORE_SET_NEXT_ID(s,v) ((s)->last=PTR((sll_semaphore_index_t)(v)))

#define THREAD_STATE_INITIALIZED 0
#define THREAD_STATE_RUNNING 1
#define THREAD_STATE_QUEUED 2
#define THREAD_STATE_WAIT_BARRIER 3
#define THREAD_STATE_WAIT_IO 4
#define THREAD_STATE_WAIT_LOCK 5
#define THREAD_STATE_WAIT_SEMAPHORE 6
#define THREAD_STATE_WAIT_THREAD 7
#define THREAD_STATE_TERMINATED 8
#define THREAD_STATE_UNDEFINED 255



typedef __SLL_U8 thread_state_t;



typedef __SLL_U32 barrier_list_length_t;



typedef __SLL_U32 lock_list_length_t;



typedef __SLL_U32 queue_length_t;



typedef __SLL_U32 semaphore_list_length_t;



typedef __SLL_U32 thread_list_length_t;



typedef __SLL_U32 wait_list_length_t;



typedef struct __THREAD_DATA{
	sll_object_t** stack;
	sll_instruction_index_t ii;
	sll_stack_offset_t si;
	sll_thread_index_t nxt;
	sll_thread_index_t wait;
	sll_object_t* ret;
	sll_call_stack_t c_st;
	sll_char_t tm;
	thread_state_t st;
	sll_bool_t suspended;
} thread_data_t;



typedef struct __LOCK{
	sll_thread_index_t lock;
	sll_thread_index_t first;
	thread_data_t* last;
} lock_t;



typedef struct __SEMAPHORE{
	sll_semaphore_counter_t count;
	sll_thread_index_t first;
	thread_data_t* last;
} semaphore_t;



typedef struct __BARRIER{
	sll_barrier_counter_t count;
	sll_thread_index_t first;
	thread_data_t* last;
} barrier_t;



extern thread_data_t* _scheduler_current_thread;



void _scheduler_deinit(void);



thread_data_t* _scheduler_get_thread(sll_thread_index_t t);



void _scheduler_init(void);



sll_thread_index_t _scheduler_new_thread(void);



void _scheduler_queue_next(void);



sll_thread_index_t _scheduler_queue_pop(void);



void _scheduler_queue_thread(sll_thread_index_t t);



void _scheduler_set_thread(sll_thread_index_t t);



void _scheduler_terminate_thread(sll_object_t* ret);



sll_bool_t _scheduler_wait_barrier(sll_integer_t w,sll_integer_t v,sll_bool_t g);



sll_bool_t _scheduler_wait_lock(sll_integer_t w);



sll_bool_t _scheduler_wait_semaphore(sll_integer_t w);



sll_bool_t _scheduler_wait_thread(sll_integer_t w);



#endif

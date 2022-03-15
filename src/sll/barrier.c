#include <sll/_internal/barrier.h>
#include <sll/_internal/scheduler.h>
#include <sll/common.h>
#include <sll/memory.h>
#include <sll/scheduler.h>
#include <sll/thread.h>
#include <sll/types.h>



static barrier_t* _barrier_data;
static sll_barrier_index_t _barrier_next;
static barrier_list_length_t _barrier_len;



static void _queue_barrier(barrier_t* b){
	if (b->first==SLL_UNKNOWN_THREAD_INDEX){
		return;
	}
	SLL_UNIMPLEMENTED();
}



void _barrier_deinit(void){
	sll_deallocate(_barrier_data);
}



void _barrier_init(void){
	_barrier_data=NULL;
	_barrier_next=BARRIER_UNUSED;
	_barrier_len=0;
}



sll_bool_t _barrier_wait(sll_integer_t w,sll_integer_t v,sll_bool_t g){
	if (w<0||w>=_barrier_len||(_barrier_data+w)->count==BARRIER_UNUSED){
		return 0;
	}
	if ((_barrier_data+w)->count==v||(g&&(_barrier_data+w)->count>v)){
		return 0;
	}
	if ((_barrier_data+w)->first==SLL_UNKNOWN_THREAD_INDEX){
		(_barrier_data+w)->first=_scheduler_current_thread_index;
	}
	else{
		(_barrier_data+w)->last->nxt=_scheduler_current_thread_index;
	}
	(_barrier_data+w)->last=_scheduler_current_thread;
	_scheduler_current_thread->nxt=SLL_UNKNOWN_THREAD_INDEX;
	_scheduler_current_thread->st=THREAD_STATE_WAIT_BARRIER;
	_scheduler_current_thread_index=SLL_UNKNOWN_THREAD_INDEX;
	return 1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_barrier_index_t sll_barrier_create(void){
	sll_barrier_index_t o=_barrier_next;
	if (o==BARRIER_UNUSED){
		o=_barrier_len;
		_barrier_len++;
		_barrier_data=sll_reallocate(_barrier_data,_barrier_len*sizeof(barrier_t));
	}
	else{
		_barrier_next=BARRIER_GET_NEXT_ID(_barrier_data+o);
	}
	(_barrier_data+o)->count=0;
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_barrier_delete(sll_barrier_index_t b){
	if (b>=_barrier_len||(_barrier_data+b)->count==BARRIER_UNUSED){
		return 0;
	}
	(_barrier_data+b)->count=BARRIER_UNUSED;
	BARRIER_SET_NEXT_ID(_barrier_data+b,_barrier_next);
	_barrier_next=b;
	return 1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_barrier_counter_t sll_barrier_increase(sll_barrier_index_t b){
	if (b>=_barrier_len||(_barrier_data+b)->count==BARRIER_UNUSED){
		return 0;
	}
	(_barrier_data+b)->count++;
	_queue_barrier(_barrier_data+b);
	return (_barrier_data+b)->count;
}




__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_barrier_reset(sll_barrier_index_t b){
	if (b>=_barrier_len||(_barrier_data+b)->count==BARRIER_UNUSED){
		return 0;
	}
	if ((_barrier_data+b)->count){
		(_barrier_data+b)->count=0;
		_queue_barrier(_barrier_data+b);
	}
	return 1;
}

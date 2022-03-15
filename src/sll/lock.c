#include <sll/_internal/lock.h>
#include <sll/_internal/scheduler.h>
#include <sll/_internal/thread.h>
#include <sll/common.h>
#include <sll/memory.h>
#include <sll/scheduler.h>
#include <sll/thread.h>
#include <sll/types.h>



static lock_t* _lock_data;
static sll_lock_index_t _lock_next;
static lock_list_length_t _lock_len;



void _lock_deinit(void){
	sll_deallocate(_lock_data);
}



void _lock_init(void){
	_lock_data=NULL;
	_lock_next=LOCK_UNUSED;
	_lock_len=0;
}



sll_bool_t _lock_wait(sll_integer_t w){
	if (w<0||w>=_lock_len||(_lock_data+w)->lock==LOCK_UNUSED){
		return 0;
	}
	SLL_ASSERT((_lock_data+w)->lock!=_scheduler_current_thread_index);
	if ((_lock_data+w)->lock==SLL_UNKNOWN_THREAD_INDEX){
		(_lock_data+w)->lock=_scheduler_current_thread_index;
		(_lock_data+w)->first=SLL_UNKNOWN_THREAD_INDEX;
		return 0;
	}
	if ((_lock_data+w)->first==SLL_UNKNOWN_THREAD_INDEX){
		(_lock_data+w)->first=_scheduler_current_thread_index;
	}
	else{
		(_lock_data+w)->last->nxt=_scheduler_current_thread_index;
	}
	(_lock_data+w)->last=_scheduler_current_thread;
	_scheduler_current_thread->nxt=SLL_UNKNOWN_THREAD_INDEX;
	_scheduler_current_thread->st=THREAD_STATE_WAIT_LOCK;
	_scheduler_current_thread_index=SLL_UNKNOWN_THREAD_INDEX;
	return 1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_lock_index_t sll_lock_create(void){
	sll_lock_index_t o=_lock_next;
	if (o==LOCK_UNUSED){
		o=_lock_len;
		_lock_len++;
		_lock_data=sll_reallocate(_lock_data,_lock_len*sizeof(lock_t));
	}
	else{
		_lock_next=LOCK_GET_NEXT_ID(_lock_data+o);
	}
	(_lock_data+o)->lock=SLL_UNKNOWN_THREAD_INDEX;
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_lock_delete(sll_lock_index_t l){
	if (l>=_lock_len||(_lock_data+l)->lock==LOCK_UNUSED){
		return 0;
	}
	SLL_ASSERT((_lock_data+l)->lock==SLL_UNKNOWN_THREAD_INDEX);
	(_lock_data+l)->lock=LOCK_UNUSED;
	LOCK_SET_NEXT_ID(_lock_data+l,_lock_next);
	_lock_next=l;
	return 1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_lock_release(sll_lock_index_t l){
	if (l>=_lock_len||(_lock_data+l)->lock==LOCK_UNUSED||(_lock_data+l)->lock==SLL_UNKNOWN_THREAD_INDEX){
		return 0;
	}
	(_lock_data+l)->lock=(_lock_data+l)->first;
	if ((_lock_data+l)->first==SLL_UNKNOWN_THREAD_INDEX){
		return 1;
	}
	thread_data_t* thr=*(_thread_data+(_lock_data+l)->lock);
	(_lock_data+l)->first=thr->nxt;
	thr->st=THREAD_STATE_QUEUED;
	_scheduler_queue_thread(0,(_lock_data+l)->lock);
	return 1;
}

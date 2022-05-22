#include <sll/_internal/lock.h>
#include <sll/_internal/scheduler.h>
#include <sll/_internal/thread.h>
#include <sll/common.h>
#include <sll/lock.h>
#include <sll/memory.h>
#include <sll/platform/lock.h>
#include <sll/scheduler.h>
#include <sll/thread.h>
#include <sll/types.h>



static sll_lock_handle_t _lock_lock;
static lock_t* _lock_data;
static sll_lock_index_t _lock_next;
static lock_list_length_t _lock_len;



void _lock_deinit(void){
	sll_deallocate(_lock_data);
	SLL_CRITICAL(sll_platform_lock_delete(_lock_lock));
}



void _lock_init(void){
	_lock_lock=sll_platform_lock_create(NULL);
	_lock_data=NULL;
	_lock_next=LOCK_UNUSED;
	_lock_len=0;
}



sll_bool_t _lock_wait(sll_integer_t lock_index){
	if (lock_index<0||lock_index>=_lock_len||(_lock_data+lock_index)->lock==LOCK_UNUSED){
		return 0;
	}
	SLL_ASSERT((_lock_data+lock_index)->lock!=_scheduler_current_thread_index);
	if ((_lock_data+lock_index)->lock==SLL_UNKNOWN_THREAD_INDEX){
		(_lock_data+lock_index)->lock=_scheduler_current_thread_index;
		(_lock_data+lock_index)->first=SLL_UNKNOWN_THREAD_INDEX;
		return 0;
	}
	if ((_lock_data+lock_index)->first==SLL_UNKNOWN_THREAD_INDEX){
		(_lock_data+lock_index)->first=_scheduler_current_thread_index;
	}
	else{
		(_lock_data+lock_index)->last->next=_scheduler_current_thread_index;
	}
	(_lock_data+lock_index)->last=_scheduler_current_thread;
	_scheduler_current_thread->next=SLL_UNKNOWN_THREAD_INDEX;
	_scheduler_current_thread->state=THREAD_STATE_WAIT_LOCK;
	_scheduler_current_thread_index=SLL_UNKNOWN_THREAD_INDEX;
	return 1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_lock_index_t sll_lock_create(void){
	SLL_CRITICAL_ERROR(sll_platform_lock_acquire(_lock_lock));
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
	SLL_CRITICAL(sll_platform_lock_release(_lock_lock));
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_lock_delete(sll_lock_index_t lock_index){
	if (lock_index>=_lock_len||(_lock_data+lock_index)->lock==LOCK_UNUSED){
		return 0;
	}
	SLL_CRITICAL_ERROR(sll_platform_lock_acquire(_lock_lock));
	SLL_ASSERT((_lock_data+lock_index)->lock==SLL_UNKNOWN_THREAD_INDEX);
	(_lock_data+lock_index)->lock=LOCK_UNUSED;
	LOCK_SET_NEXT_ID(_lock_data+lock_index,_lock_next);
	_lock_next=lock_index;
	SLL_CRITICAL(sll_platform_lock_release(_lock_lock));
	return 1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_lock_release(sll_lock_index_t lock_index){
	if (lock_index>=_lock_len||(_lock_data+lock_index)->lock==LOCK_UNUSED||(_lock_data+lock_index)->lock==SLL_UNKNOWN_THREAD_INDEX){
		return 0;
	}
	(_lock_data+lock_index)->lock=(_lock_data+lock_index)->first;
	if ((_lock_data+lock_index)->first==SLL_UNKNOWN_THREAD_INDEX){
		return 1;
	}
	thread_data_t* thr=*(_thread_data+(_lock_data+lock_index)->lock);
	(_lock_data+lock_index)->first=thr->next;
	thr->state=THREAD_STATE_QUEUED;
	_scheduler_queue_thread((_lock_data+lock_index)->lock);
	return 1;
}

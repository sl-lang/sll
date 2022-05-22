#include <sll/_internal/scheduler.h>
#include <sll/_internal/semaphore.h>
#include <sll/common.h>
#include <sll/memory.h>
#include <sll/platform/lock.h>
#include <sll/scheduler.h>
#include <sll/semaphore.h>
#include <sll/thread.h>
#include <sll/types.h>



static sll_lock_handle_t _semaphore_lock;
static semaphore_t* _semaphore_data;
static sll_semaphore_index_t _semaphore_next;
static semaphore_list_length_t _semaphore_len;



void _semaphore_deinit(void){
	sll_deallocate(_semaphore_data);
	SLL_CRITICAL(sll_platform_lock_delete(_semaphore_lock));
}



void _semaphore_init(void){
	_semaphore_lock=sll_platform_lock_create(NULL);
	_semaphore_data=NULL;
	_semaphore_next=SEMAPHORE_UNUSED;
	_semaphore_len=0;
}



sll_bool_t _semaphore_wait(sll_integer_t sempahore_index){
	if (sempahore_index<0||sempahore_index>=_semaphore_len||(_semaphore_data+sempahore_index)->count==SEMAPHORE_UNUSED){
		return 0;
	}
	if ((_semaphore_data+sempahore_index)->count){
		(_semaphore_data+sempahore_index)->count--;
		(_semaphore_data+sempahore_index)->first=SLL_UNKNOWN_THREAD_INDEX;
		return 0;
	}
	if ((_semaphore_data+sempahore_index)->first==SLL_UNKNOWN_THREAD_INDEX){
		(_semaphore_data+sempahore_index)->first=_scheduler_current_thread_index;
	}
	else{
		(_semaphore_data+sempahore_index)->last->next=_scheduler_current_thread_index;
	}
	(_semaphore_data+sempahore_index)->last=_scheduler_current_thread;
	_scheduler_current_thread->next=SLL_UNKNOWN_THREAD_INDEX;
	_scheduler_current_thread->state=THREAD_STATE_WAIT_SEMAPHORE;
	_scheduler_current_thread_index=SLL_UNKNOWN_THREAD_INDEX;
	return 1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_semaphore_index_t sll_semaphore_create(sll_semaphore_counter_t count){
	SLL_CRITICAL_ERROR(sll_platform_lock_acquire(_semaphore_lock));
	sll_semaphore_index_t o=_semaphore_next;
	if (o==SEMAPHORE_UNUSED){
		o=_semaphore_len;
		_semaphore_len++;
		_semaphore_data=sll_reallocate(_semaphore_data,_semaphore_len*sizeof(semaphore_t));
	}
	else{
		_semaphore_next=SEMAPHORE_GET_NEXT_ID(_semaphore_data+o);
	}
	(_semaphore_data+o)->count=count;
	(_semaphore_data+o)->first=SLL_UNKNOWN_THREAD_INDEX;
	SLL_CRITICAL(sll_platform_lock_release(_semaphore_lock));
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_semaphore_delete(sll_semaphore_index_t sempahore_index){
	if (sempahore_index>=_semaphore_len||(_semaphore_data+sempahore_index)->count==SEMAPHORE_UNUSED){
		return 0;
	}
	SLL_CRITICAL_ERROR(sll_platform_lock_acquire(_semaphore_lock));
	(_semaphore_data+sempahore_index)->count=SEMAPHORE_UNUSED;
	SEMAPHORE_SET_NEXT_ID(_semaphore_data+sempahore_index,_semaphore_next);
	_semaphore_next=sempahore_index;
	SLL_CRITICAL(sll_platform_lock_release(_semaphore_lock));
	return 1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_semaphore_release(sll_semaphore_index_t sempahore_index){
	if (sempahore_index>=_semaphore_len||(_semaphore_data+sempahore_index)->count==SEMAPHORE_UNUSED){
		return 0;
	}
	if ((_semaphore_data+sempahore_index)->first==SLL_UNKNOWN_THREAD_INDEX){
		(_semaphore_data+sempahore_index)->count++;
		return 1;
	}
	thread_data_t* thr=*(_thread_data+(_semaphore_data+sempahore_index)->first);
	thr->state=THREAD_STATE_QUEUED;
	_scheduler_queue_thread((_semaphore_data+sempahore_index)->first);
	(_semaphore_data+sempahore_index)->first=thr->next;
	return 1;
}

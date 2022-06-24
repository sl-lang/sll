#include <sll/_internal/lock.h>
#include <sll/_internal/scheduler.h>
#include <sll/_internal/thread.h>
#include <sll/common.h>
#include <sll/container.h>
#include <sll/lock.h>
#include <sll/memory.h>
#include <sll/platform/lock.h>
#include <sll/scheduler.h>
#include <sll/thread.h>
#include <sll/types.h>



static sll_lock_handle_t _lock_lock;
static sll_handle_container_t _lock_data;



void _lock_deinit(void){
	SLL_HANDLE_CONTAINER_CLEAR(&_lock_data);
	SLL_CRITICAL(sll_platform_lock_delete(_lock_lock));
}



void _lock_init(void){
	_lock_lock=sll_platform_lock_create(NULL);
	SLL_HANDLE_CONTAINER_INIT(&_lock_data);
}



sll_bool_t _lock_wait(sll_integer_t lock_index){
	if (lock_index<0||!SLL_HANDLE_CONTAINER_CHECK(&_lock_data,(sll_size_t)lock_index)){
		return 0;
	}
	lock_t* data=*(_lock_data.data+lock_index);
	SLL_ASSERT(data->lock!=_scheduler_current_thread_index);
	if (data->lock==SLL_UNKNOWN_THREAD_INDEX){
		data->lock=_scheduler_current_thread_index;
		data->first=SLL_UNKNOWN_THREAD_INDEX;
		return 0;
	}
	if (data->first==SLL_UNKNOWN_THREAD_INDEX){
		data->first=_scheduler_current_thread_index;
	}
	else{
		data->last->next=_scheduler_current_thread_index;
	}
	data->last=_scheduler_current_thread;
	_scheduler_current_thread->next=SLL_UNKNOWN_THREAD_INDEX;
	_scheduler_current_thread->state=THREAD_STATE_WAIT_LOCK;
	_scheduler_current_thread_index=SLL_UNKNOWN_THREAD_INDEX;
	return 1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_lock_index_t sll_lock_create(void){
	SLL_CRITICAL_ERROR(sll_platform_lock_acquire(_lock_lock));
	sll_size_t o;
	SLL_HANDLE_CONTAINER_ALLOC(&_lock_data,&o);
	lock_t* data=sll_allocate(sizeof(lock_t));
	data->lock=SLL_UNKNOWN_THREAD_INDEX;
	*(_lock_data.data+o)=data;
	SLL_CRITICAL(sll_platform_lock_release(_lock_lock));
	return (sll_lock_index_t)o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_lock_delete(sll_lock_index_t lock_index){
	if (!SLL_HANDLE_CONTAINER_CHECK(&_lock_data,lock_index)){
		return 0;
	}
	SLL_CRITICAL_ERROR(sll_platform_lock_acquire(_lock_lock));
	sll_deallocate(*(_lock_data.data+lock_index));
	SLL_HANDLE_CONTAINER_DEALLOC(&_lock_data,lock_index);
	SLL_CRITICAL(sll_platform_lock_release(_lock_lock));
	return 1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_lock_release(sll_lock_index_t lock_index){
	if (!SLL_HANDLE_CONTAINER_CHECK(&_lock_data,lock_index)){
		return 0;
	}
	lock_t* data=*(_lock_data.data+lock_index);
	if (data->lock==SLL_UNKNOWN_THREAD_INDEX){
		return 0;
	}
	data->lock=data->first;
	if (data->first==SLL_UNKNOWN_THREAD_INDEX){
		return 1;
	}
	thread_data_t* thr=*(_thread_data+data->lock);
	data->first=thr->next;
	thr->state=THREAD_STATE_QUEUED;
	_scheduler_queue_thread(data->lock);
	return 1;
}

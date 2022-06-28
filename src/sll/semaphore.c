#include <sll/_internal/scheduler.h>
#include <sll/_internal/semaphore.h>
#include <sll/common.h>
#include <sll/container.h>
#include <sll/memory.h>
#include <sll/platform/lock.h>
#include <sll/scheduler.h>
#include <sll/semaphore.h>
#include <sll/thread.h>
#include <sll/types.h>



static sll_lock_handle_t _semaphore_lock;
static sll_handle_container_t _semaphore_data;



void _semaphore_deinit(void){
	SLL_HANDLE_CONTAINER_CLEAR(&_semaphore_data);
	SLL_CRITICAL(sll_platform_lock_delete(_semaphore_lock));
}



void _semaphore_init(void){
	_semaphore_lock=sll_platform_lock_create(NULL);
	SLL_HANDLE_CONTAINER_INIT(&_semaphore_data);
}



sll_bool_t _semaphore_wait(sll_integer_t sempahore_index){
	if (sempahore_index<0||!SLL_HANDLE_CONTAINER_CHECK(&_semaphore_data,(sll_size_t)sempahore_index)){
		return 0;
	}
	semaphore_t* data=*(_semaphore_data.data+sempahore_index);
	if (data->count){
		data->count--;
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
	_scheduler_current_thread->state=THREAD_STATE_WAIT_SEMAPHORE;
	_scheduler_current_thread_index=SLL_UNKNOWN_THREAD_INDEX;
	return 1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_semaphore_index_t sll_semaphore_create(sll_semaphore_counter_t count){
	SLL_CRITICAL_ERROR(sll_platform_lock_acquire(_semaphore_lock));
	sll_size_t o;
	SLL_HANDLE_CONTAINER_ALLOC(&_semaphore_data,&o);
	semaphore_t* data=sll_allocate(sizeof(semaphore_t));
	data->count=count;
	data->first=SLL_UNKNOWN_THREAD_INDEX;
	*(_semaphore_data.data+o)=data;
	SLL_CRITICAL(sll_platform_lock_release(_semaphore_lock));
	return (sll_semaphore_index_t)o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_semaphore_delete(sll_semaphore_index_t sempahore_index){
	if (!SLL_HANDLE_CONTAINER_CHECK(&_semaphore_data,sempahore_index)){
		return 0;
	}
	SLL_CRITICAL_ERROR(sll_platform_lock_acquire(_semaphore_lock));
	sll_deallocate(*(_semaphore_data.data+sempahore_index));
	SLL_HANDLE_CONTAINER_DEALLOC(&_semaphore_data,sempahore_index);
	SLL_CRITICAL(sll_platform_lock_release(_semaphore_lock));
	return 1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_semaphore_release(sll_semaphore_index_t sempahore_index){
	if (!SLL_HANDLE_CONTAINER_CHECK(&_semaphore_data,sempahore_index)){
		return 0;
	}
	semaphore_t* data=*(_semaphore_data.data+sempahore_index);
	if (data->first==SLL_UNKNOWN_THREAD_INDEX){
		data->count++;
		return 1;
	}
	thread_data_t* thr=*(_thread_data.data+data->first);
	thr->state=THREAD_STATE_QUEUED;
	_scheduler_queue_thread(data->first);
	data->first=thr->next;
	return 1;
}

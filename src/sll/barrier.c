#include <sll/_internal/barrier.h>
#include <sll/_internal/scheduler.h>
#include <sll/barrier.h>
#include <sll/common.h>
#include <sll/container.h>
#include <sll/memory.h>
#include <sll/platform/lock.h>
#include <sll/scheduler.h>
#include <sll/thread.h>
#include <sll/types.h>



static sll_handle_container_t _barrier_data;
static sll_lock_handle_t _barrier_lock;



static void _queue_barrier(barrier_t* b){
	if (b->first==SLL_UNKNOWN_THREAD_INDEX){
		return;
	}
	SLL_UNIMPLEMENTED();
}



void _barrier_deinit(void){
	SLL_HANDLE_CONTAINER_CLEAR(&_barrier_data);
	SLL_CRITICAL(sll_platform_lock_delete(_barrier_lock));
}



void _barrier_init(void){
	_barrier_lock=sll_platform_lock_create(NULL);
	SLL_HANDLE_CONTAINER_INIT(&_barrier_data);
}



sll_bool_t _barrier_wait(sll_integer_t barrier_index,sll_integer_t value,sll_bool_t greate_or_equal){
	if (barrier_index<0||!SLL_HANDLE_CONTAINER_CHECK(&_barrier_data,(sll_size_t)barrier_index)){
		return 0;
	}
	barrier_t* data=*(_barrier_data.data+barrier_index);
	if (data->count==value||(greate_or_equal&&data->count>value)){
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
	_scheduler_current_thread->state=THREAD_STATE_WAIT_BARRIER;
	_scheduler_current_thread_index=SLL_UNKNOWN_THREAD_INDEX;
	return 1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_barrier_index_t sll_barrier_create(void){
	SLL_CRITICAL_ERROR(sll_platform_lock_acquire(_barrier_lock));
	sll_size_t out;
	SLL_HANDLE_CONTAINER_ALLOC(&_barrier_data,&out);
	barrier_t* data=sll_allocate(sizeof(barrier_t));
	*(_barrier_data.data+out)=data;
	data->count=0;
	SLL_CRITICAL(sll_platform_lock_release(_barrier_lock));
	return (sll_barrier_index_t)out;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_barrier_delete(sll_barrier_index_t barrier_index){
	if (!SLL_HANDLE_CONTAINER_CHECK(&_barrier_data,barrier_index)){
		return 0;
	}
	SLL_CRITICAL_ERROR(sll_platform_lock_acquire(_barrier_lock));
	sll_deallocate(*(_barrier_data.data+barrier_index));
	SLL_HANDLE_CONTAINER_DEALLOC(&_barrier_data,barrier_index);
	SLL_CRITICAL(sll_platform_lock_release(_barrier_lock));
	return 1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_barrier_counter_t sll_barrier_increase(sll_barrier_index_t barrier_index){
	if (!SLL_HANDLE_CONTAINER_CHECK(&_barrier_data,barrier_index)){
		return 0;
	}
	barrier_t* data=*(_barrier_data.data+barrier_index);
	data->count++;
	_queue_barrier(data);
	return data->count;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_barrier_reset(sll_barrier_index_t barrier_index){
	if (!SLL_HANDLE_CONTAINER_CHECK(&_barrier_data,barrier_index)){
		return 0;
	}
	barrier_t* data=*(_barrier_data.data+barrier_index);
	if (data->count){
		data->count=0;
		_queue_barrier(data);
	}
	return 1;
}

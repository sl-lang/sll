#include <sll/_internal/scheduler.h>
#include <sll/_internal/thread.h>
#include <sll/array.h>
#include <sll/barrier.h>
#include <sll/common.h>
#include <sll/error.h>
#include <sll/gc.h>
#include <sll/lock.h>
#include <sll/new_object.h>
#include <sll/sandbox.h>
#include <sll/semaphore.h>
#include <sll/static_object.h>
#include <sll/thread.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_thread_create(sll_integer_t function,const sll_array_t* args,sll_thread_index_t* out){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)){
		return SLL_ERROR_FROM_SANDBOX(SLL_SANDBOX_FLAG_DISABLE_THREADS);
	}
	*out=sll_thread_create(function,args->data,args->length);
	return (sll_thread_start(*out)?SLL_NO_ERROR:SLL_UNMAPPED_SYSTEM_ERROR|SLL_ERROR_FLAG_SYSTEM);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_thread_create_barrier(sll_barrier_index_t* out){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)){
		return SLL_ERROR_FROM_SANDBOX(SLL_SANDBOX_FLAG_DISABLE_THREADS);
	}
	*out=sll_barrier_create();
	return SLL_NO_ERROR;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_thread_create_lock(sll_lock_index_t* out){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)){
		return SLL_ERROR_FROM_SANDBOX(SLL_SANDBOX_FLAG_DISABLE_THREADS);
	}
	*out=sll_lock_create();
	return SLL_NO_ERROR;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_thread_create_semaphore(sll_semaphore_counter_t count,sll_semaphore_index_t* out){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)){
		return SLL_ERROR_FROM_SANDBOX(SLL_SANDBOX_FLAG_DISABLE_THREADS);
	}
	*out=sll_semaphore_create(count);
	return SLL_NO_ERROR;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_thread_delete(sll_thread_index_t thread_index){
	return (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)?0:sll_thread_delete(thread_index));
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_thread_delete_barrier(sll_barrier_index_t barrier_index){
	return (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)?0:sll_barrier_delete(barrier_index));
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_thread_delete_lock(sll_lock_index_t lock_index){
	return (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)?0:sll_lock_delete(lock_index));
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_thread_delete_semaphore(sll_semaphore_index_t semaphore_index){
	return (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)?0:sll_semaphore_delete(semaphore_index));
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_thread_get_internal_data(sll_thread_index_t thread_index,sll_array_t* out){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)){
		goto _error;
	}
	thread_data_t* thr=_thread_get(thread_index);
	if (thr){
		sll_new_object_array(SLL_CHAR("hh"),out,thr->instruction_index,thr->stack_index);
		return;
	}
_error:
	sll_new_object_array(SLL_CHAR("00"),out);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_barrier_counter_t sll_api_thread_increase_barrier(sll_barrier_index_t barrier_index){
	return (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)?0:sll_barrier_increase(barrier_index));
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_thread_release_lock(sll_lock_index_t lock_index){
	return (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)?0:sll_lock_release(lock_index));
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_thread_release_semaphore(sll_semaphore_index_t semaphore_index){
	return (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)?0:sll_semaphore_release(semaphore_index));
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_barrier_counter_t sll_api_thread_reset_barrier(sll_barrier_index_t barrier_index){
	return (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)?0:sll_barrier_reset(barrier_index));
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_thread_restart(sll_thread_index_t thread_index){
	return (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)?0:sll_thread_restart(thread_index));
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_thread_suspend(sll_thread_index_t thread_index){
	return (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)?0:sll_thread_suspend(thread_index));
}

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



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_thread_create(sll_integer_t fn,const sll_array_t* args,sll_thread_index_t* out){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)){
		return SLL_ERROR_FROM_SANDBOX(SLL_SANDBOX_FLAG_DISABLE_THREADS);
	}
	*out=sll_thread_create(fn,args->data,args->length);
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



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_thread_create_semaphore(sll_semaphore_counter_t cnt,sll_semaphore_index_t* out){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)){
		return SLL_ERROR_FROM_SANDBOX(SLL_SANDBOX_FLAG_DISABLE_THREADS);
	}
	*out=sll_semaphore_create(cnt);
	return SLL_NO_ERROR;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_thread_delete(sll_thread_index_t tid){
	return (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)?0:sll_thread_delete(tid));
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_thread_delete_barrier(sll_barrier_index_t bid){
	return (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)?0:sll_barrier_delete(bid));
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_thread_delete_lock(sll_lock_index_t lid){
	return (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)?0:sll_lock_delete(lid));
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_thread_delete_semaphore(sll_semaphore_index_t sid){
	return (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)?0:sll_semaphore_delete(sid));
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_thread_get_internal_data(sll_thread_index_t tid,sll_array_t* out){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)){
		goto _error;
	}
	thread_data_t* thr=_thread_get(tid);
	if (thr){
		sll_new_object_array(SLL_CHAR("hh"),out,thr->instruction_index,thr->stack_index);
		return;
	}
_error:
	sll_new_object_array(SLL_CHAR("00"),out);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_barrier_counter_t sll_api_thread_increase_barrier(sll_barrier_index_t bid){
	return (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)?0:sll_barrier_increase(bid));
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_thread_release_lock(sll_lock_index_t lid){
	return (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)?0:sll_lock_release(lid));
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_thread_release_semaphore(sll_semaphore_index_t sid){
	return (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)?0:sll_semaphore_release(sid));
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_barrier_counter_t sll_api_thread_reset_barrier(sll_barrier_index_t bid){
	return (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)?0:sll_barrier_reset(bid));
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_thread_restart(sll_thread_index_t tid){
	return (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)?0:sll_thread_restart(tid));
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_thread_suspend(sll_thread_index_t tid){
	return (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)?0:sll_thread_suspend(tid));
}

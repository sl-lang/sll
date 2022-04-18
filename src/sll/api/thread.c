#include <sll/_internal/thread.h>
#include <sll/array.h>
#include <sll/barrier.h>
#include <sll/common.h>
#include <sll/lock.h>
#include <sll/new_object.h>
#include <sll/sandbox.h>
#include <sll/semaphore.h>
#include <sll/static_object.h>
#include <sll/thread.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_thread_create(sll_integer_t fn,const sll_array_t* args){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)){
		return -1;
	}
	sll_thread_index_t o=sll_thread_create(fn,args->v,args->l);
	return (!sll_thread_start(o)?-1:o);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_thread_create_barrier(void){
	return (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)?-1:sll_barrier_create());
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_thread_create_lock(void){
	return (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)?-1:sll_lock_create());
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_thread_create_semaphore(sll_semaphore_counter_t cnt){
	return (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)?-1:sll_semaphore_create(cnt));
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
		sll_new_object_array(SLL_CHAR("hh"),out,thr->ii,thr->si);
		return;
	}
_error:
	sll_new_object_array(SLL_CHAR("00"),out);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_thread_increase_barrier(sll_barrier_index_t bid){
	return (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)?0:sll_barrier_increase(bid));
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_thread_release_lock(sll_lock_index_t lid){
	return (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)?0:sll_lock_release(lid));
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_thread_release_semaphore(sll_semaphore_index_t sid){
	return (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)?0:sll_semaphore_release(sid));
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_thread_reset_barrier(sll_barrier_index_t bid){
	return (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)?0:sll_barrier_reset(bid));
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_thread_restart(sll_thread_index_t tid){
	return (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)?0:sll_thread_restart(tid));
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_thread_suspend(sll_thread_index_t tid){
	return (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)?0:sll_thread_suspend(tid));
}

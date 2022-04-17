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



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_thread_create(sll_integer_t a,sll_array_t* b){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)){
		return -1;
	}
	sll_thread_index_t o=sll_thread_create(a,b->v,b->l);
	return (!sll_thread_start(o)?-1:o);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_thread_create_barrier(void){
	return (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)?-1:sll_barrier_create());
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_thread_create_lock(void){
	return (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)?-1:sll_lock_create());
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_thread_create_semaphore(sll_integer_t a){
	return (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)?-1:sll_semaphore_create((a<0?0:(sll_semaphore_counter_t)a)));
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_thread_delete(sll_integer_t a){
	return (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)||a<0||a>SLL_MAX_THREAD_INDEX?0:sll_thread_delete((sll_thread_index_t)a));
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_thread_delete_barrier(sll_integer_t a){
	return (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)||a<0||a>SLL_MAX_BARRIER_INDEX?0:sll_barrier_delete((sll_barrier_index_t)a));
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_thread_delete_lock(sll_integer_t a){
	return (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)||a<0||a>SLL_MAX_LOCK_INDEX?0:sll_lock_delete((sll_lock_index_t)a));
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_thread_delete_semaphore(sll_integer_t a){
	return (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)||a<0||a>SLL_MAX_SEMAPHORE_INDEX?0:sll_semaphore_delete((sll_semaphore_index_t)a));
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_thread_get_internal_data(sll_integer_t a,sll_array_t* out){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)||a<0||a>SLL_MAX_THREAD_INDEX){
		goto _error;
	}
	thread_data_t* thr=_thread_get((sll_thread_index_t)a);
	if (thr){
		sll_new_object_array(SLL_CHAR("hh"),out,thr->ii,thr->si);
		return;
	}
_error:
	sll_new_object_array(SLL_CHAR("00"),out);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_thread_increase_barrier(sll_integer_t a){
	return (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)||a<0||a>SLL_MAX_BARRIER_INDEX?0:sll_barrier_increase((sll_barrier_index_t)a));
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_thread_release_lock(sll_integer_t a){
	return (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)||a<0||a>SLL_MAX_LOCK_INDEX?0:sll_lock_release((sll_lock_index_t)a));
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_thread_release_semaphore(sll_integer_t a){
	return (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)||a<0||a>SLL_MAX_SEMAPHORE_INDEX?0:sll_semaphore_release((sll_semaphore_index_t)a));
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_thread_reset_barrier(sll_integer_t a){
	return (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)||a<0||a>SLL_MAX_BARRIER_INDEX?0:sll_barrier_reset((sll_barrier_index_t)a));
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_thread_restart(sll_integer_t a){
	return (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)||a<0||a>SLL_MAX_THREAD_INDEX?0:sll_thread_restart((sll_thread_index_t)a));
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_thread_suspend(sll_integer_t a){
	return (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)||a<0||a>SLL_MAX_THREAD_INDEX?0:sll_thread_suspend((sll_thread_index_t)a));
}

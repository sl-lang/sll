#include <sll/_internal/common.h>
#include <sll/_internal/error.h>
#include <sll/common.h>
#include <sll/error.h>
#include <sll/memory.h>
#include <sll/platform/lock.h>
#include <sll/types.h>
#include <pthread.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_lock_acquire(sll_lock_handle_t l){
	int err=pthread_mutex_lock(l);
	return (err?err|SLL_ERROR_FLAG_SYSTEM:SLL_NO_ERROR);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_lock_handle_t sll_platform_lock_create(sll_error_t* err){
	ERROR_PTR_RESET;
	pthread_mutex_t mtx;
	int pthread_err=pthread_mutex_init(&mtx,NULL);
	if (pthread_err){
		ERROR_PTR(pthread_err|SLL_ERROR_FLAG_SYSTEM);
		return NULL;
	}
	pthread_mutex_t* o=sll_allocate(sizeof(pthread_mutex_t));
	*o=mtx;
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_lock_delete(sll_lock_handle_t l){
	int err=pthread_mutex_destroy(l);
	if (err){
		return err|SLL_ERROR_FLAG_SYSTEM;
	}
	sll_deallocate(l);
	return SLL_NO_ERROR;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_lock_release(sll_lock_handle_t l){
	int err=pthread_mutex_unlock(l);
	return (err?err|SLL_ERROR_FLAG_SYSTEM:SLL_NO_ERROR);
}

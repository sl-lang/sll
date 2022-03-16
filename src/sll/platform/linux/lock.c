#include <sll/_internal/common.h>
#include <sll/common.h>
#include <sll/memory.h>
#include <sll/types.h>
#include <pthread.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_lock_acquire(sll_lock_handle_t l){
	return !pthread_mutex_lock(l);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_lock_handle_t sll_platform_lock_create(void){
	pthread_mutex_t mtx;
	if (pthread_mutex_init(&mtx,NULL)){
		return NULL;
	}
	pthread_mutex_t* o=sll_allocate(sizeof(pthread_mutex_t));
	*o=mtx;
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_lock_delete(sll_lock_handle_t l){
	if (pthread_mutex_destroy(l)){
		return 0;
	}
	sll_deallocate(l);
	return 1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_lock_release(sll_lock_handle_t l){
	return !pthread_mutex_unlock(l);
}

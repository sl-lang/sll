#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/platform/thread.h>
#include <sll/types.h>
#include <pthread.h>
#include <semaphore.h>



static void* _execute_wrapper(void* p){
	execute_wrapper_data_t dt=*((execute_wrapper_data_t*)p);
	sem_post((sem_t*)dt.sem);
	dt.fn(dt.arg);
	return NULL;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_join_thread(sll_internal_thread_index_t tid){
	return !!pthread_join((pthread_t)tid,NULL);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_internal_thread_index_t sll_platform_start_thread(sll_internal_thread_function_t fn,void* arg){
	pthread_t o;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	sem_t sem;
	sem_init(&sem,0,0);
	execute_wrapper_data_t dt={
		fn,
		arg,
		&sem
	};
	if (pthread_create(&o,&attr,_execute_wrapper,&dt)){
		return SLL_UNKNOWN_INTERNAL_THREAD_INDEX;
	}
	sem_wait(&sem);
	sem_destroy(&sem);
	return (sll_internal_thread_index_t)o;
}

#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/platform/thread.h>
#include <sll/types.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/stat.h>



static void* _execute_wrapper(void* p){
	execute_wrapper_data_t dt=*((execute_wrapper_data_t*)p);
	sem_post((sem_t*)dt.sem);
	dt.fn(dt.arg);
	return NULL;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_join_thread(sll_internal_thread_index_t tid){
	return !pthread_join((pthread_t)tid,NULL);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_internal_thread_index_t sll_platform_start_thread(sll_internal_thread_function_t fn,void* arg){
	pthread_t o;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	execute_wrapper_data_t dt={
		fn,
		arg,
		sem_open("__sll_execute_wrapper_sync",O_CREAT,S_IRUSR|S_IWUSR,0)
	};
	if (pthread_create(&o,&attr,_execute_wrapper,&dt)){
		return SLL_UNKNOWN_INTERNAL_THREAD_INDEX;
	}
	sem_wait(dt.sem);
	sem_close(dt.sem);
	sem_unlink("__sll_execute_wrapper_sync");
	return (sll_internal_thread_index_t)o;
}

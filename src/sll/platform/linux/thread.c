#include <sll/_internal/common.h>
#include <sll/_internal/error.h>
#include <sll/_internal/platform.h>
#include <sll/common.h>
#include <sll/error.h>
#include <sll/platform/thread.h>
#include <sll/platform/util.h>
#include <sll/types.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <unistd.h>
#ifdef __SLL_BUILD_DARWIN
#include <mach/thread_act.h>
#include <mach/thread_policy.h>
#endif



static pthread_mutex_t _thread_lock;
static sll_bool_t _thread_lock_init=0;
static execute_wrapper_data_t _thread_wrapper_data;
static volatile sll_bool_t _thread_creation_lock=0;



static void* _execute_wrapper(void){
	sll_internal_thread_function_t fn=_thread_wrapper_data.function;
	void* arg=_thread_wrapper_data.arg;
	_thread_creation_lock=1;
	fn(arg);
	return NULL;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_internal_thread_index_t sll_platform_current_thread(void){
	return (sll_internal_thread_index_t)pthread_self();
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_join_thread(sll_internal_thread_index_t tid){
	int err=pthread_join((pthread_t)tid,NULL);
	return (err?err|SLL_ERROR_FLAG_SYSTEM:SLL_NO_ERROR);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_set_cpu(sll_cpu_t cpu){
	if (cpu!=SLL_CPU_ANY&&cpu>=*sll_platform_cpu_count){
		cpu=SLL_CPU_ANY;
	}
#ifdef __SLL_BUILD_DARWIN
	thread_affinity_policy_data_t dt={
		(cpu==SLL_CPU_ANY?0:cpu+1)
	};
	return (thread_policy_set(pthread_mach_thread_np(pthread_self()),THREAD_AFFINITY_POLICY,(thread_policy_t)(&dt),THREAD_AFFINITY_POLICY_COUNT)==KERN_SUCCESS?SLL_NO_ERROR:SLL_ERROR_FLAG_SYSTEM);
#else
	cpu_set_t set;
	CPU_ZERO(&set);
	if (cpu!=SLL_CPU_ANY){
		CPU_SET(cpu,&set);
	}
	else{
		for (sll_cpu_t i=0;i<*sll_platform_cpu_count;i++){
			CPU_SET(i,&set);
		}
	}
	int err=pthread_setaffinity_np(pthread_self(),sizeof(cpu_set_t),&set);
	return (err?err|SLL_ERROR_FLAG_SYSTEM:SLL_NO_ERROR);
#endif
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_internal_thread_index_t sll_platform_start_thread(sll_internal_thread_function_t fn,void* arg,sll_error_t* err){
	ERROR_PTR_RESET;
	if (!_thread_lock_init){
		int pthread_err=pthread_mutex_init(&_thread_lock,NULL);
		if (pthread_err){
			ERROR_PTR(pthread_err|SLL_ERROR_FLAG_SYSTEM);
			return SLL_UNKNOWN_INTERNAL_THREAD_INDEX;
		}
		_thread_lock_init=1;
	}
	pthread_mutex_lock(&_thread_lock);
	_thread_wrapper_data.function=fn;
	_thread_wrapper_data.arg=arg;
	pthread_t o;
	int pthread_err=pthread_create(&o,NULL,(void* (*)(void*))_execute_wrapper,NULL);
	if (pthread_err){
		ERROR_PTR(pthread_err|SLL_ERROR_FLAG_SYSTEM);
		return SLL_UNKNOWN_INTERNAL_THREAD_INDEX;
	}
	while (!_thread_creation_lock){
		asm("pause");
	}
	_thread_creation_lock=0;
	pthread_mutex_unlock(&_thread_lock);
	return (sll_internal_thread_index_t)o;
}

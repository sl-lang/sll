#include <sll/_internal/platform.h>
#include <sll/common.h>
#include <sll/platform/thread.h>
#include <sll/types.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <unistd.h>
#ifdef __SLL_BUILD_DARWIN
#include <mach/thread_policy.h>
#endif



static void* _execute_wrapper(void* p){
	execute_wrapper_data_t dt=*((execute_wrapper_data_t*)p);
	sem_post((sem_t*)dt.lck);
	dt.fn(dt.arg);
	return NULL;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_internal_thread_index_t sll_platform_current_thread(void){
	return (sll_internal_thread_index_t)pthread_self();
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_join_thread(sll_internal_thread_index_t tid){
	return !pthread_join((pthread_t)tid,NULL);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_set_cpu(sll_internal_thread_index_t tid,sll_cpu_t cpu){
	sll_cpu_t max=sysconf(_SC_NPROCESSORS_ONLN);
	if (cpu!=SLL_CPU_ANY&&cpu>=max){
		cpu=SLL_CPU_ANY;
	}
#ifdef __SLL_BUILD_DARWIN
	thread_port_t sys_tid=pthread_mach_thread_np(tid);
	if (cpu==SLL_CPU_ANY){
		SLL_UNIMPLEMENTED();
	}
	thread_affinity_policy_data_t dt={
		cpu
	};
	return (thread_policy_set(sys_tid,THREAD_AFFINITY_POLICY,(thread_policy_t)(&dt),1)==KERN_SUCCESS);
#else
	cpu_set_t set;
	CPU_ZERO(&set);
	if (cpu!=SLL_CPU_ANY){
		CPU_SET(cpu,&set);
	}
	else{
		while (max){
			max--;
			CPU_SET(max,&set);
		}
	}
	return !pthread_setaffinity_np((pthread_t)tid,sizeof(set),&set);
#endif
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_internal_thread_index_t sll_platform_start_thread(sll_internal_thread_function_t fn,void* arg){
	pthread_t o;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	execute_wrapper_data_t dt={
		fn,
		arg,
		sem_open("/__sll_execute_wrapper_sync",O_CREAT,S_IRUSR|S_IWUSR,0)
	};
	if (pthread_create(&o,&attr,_execute_wrapper,&dt)){
		return SLL_UNKNOWN_INTERNAL_THREAD_INDEX;
	}
	sem_wait(dt.lck);
	sem_close(dt.lck);
	sem_unlink("/__sll_execute_wrapper_sync");
	return (sll_internal_thread_index_t)o;
}

#include <windows.h>
#include <sll/_internal/common.h>
#include <sll/_internal/platform.h>
#include <sll/common.h>
#include <sll/platform/thread.h>
#include <sll/types.h>



static DWORD __stdcall _execute_wrapper(void* p){
	execute_wrapper_data_t dt=*((execute_wrapper_data_t*)p);
	if (!ReleaseSemaphore(dt.lck,1,NULL)){
		SLL_UNIMPLEMENTED();
	}
	dt.fn(dt.arg);
	return 0;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_internal_thread_index_t sll_platform_current_thread(void){
	return (sll_internal_thread_index_t)GetCurrentThread();
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_join_thread(sll_internal_thread_index_t tid){
	return (WaitForSingleObject((HANDLE)tid,INFINITE)==WAIT_OBJECT_0);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_set_cpu(sll_internal_thread_index_t tid,sll_cpu_t cpu){
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	return !!SetThreadAffinityMask((HANDLE)tid,(cpu==SLL_CPU_ANY||cpu>=si.dwNumberOfProcessors?0xffffffffffffffffull:1ull<<cpu));
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_internal_thread_index_t sll_platform_start_thread(sll_internal_thread_function_t fn,void* arg){
	execute_wrapper_data_t dt={
		fn,
		arg,
		CreateSemaphoreA(NULL,0,1,NULL)
	};
	HANDLE o=CreateThread(NULL,0,_execute_wrapper,&dt,0,NULL);
	if (!o){
		return SLL_UNKNOWN_INTERNAL_THREAD_INDEX;
	}
	if (WaitForSingleObject(dt.lck,INFINITE)!=WAIT_OBJECT_0){
		SLL_UNIMPLEMENTED();
	}
	CloseHandle(dt.lck);
	return (sll_internal_thread_index_t)o;
}

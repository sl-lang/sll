#include <windows.h>
#include <sll/common.h>
#include <sll/error.h>
#include <sll/platform/lock.h>
#include <sll/platform/util.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_lock_acquire(sll_lock_handle_t l){
	return (WaitForSingleObject((HANDLE)l,INFINITE)==WAIT_OBJECT_0?SLL_NO_ERROR:sll_platform_get_error());
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_lock_handle_t sll_platform_lock_create(void){
	return (sll_lock_handle_t)CreateMutexA(NULL,FALSE,NULL);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_lock_delete(sll_lock_handle_t l){
	return (CloseHandle((HANDLE)l)?SLL_NO_ERROR:sll_platform_get_error());
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_lock_release(sll_lock_handle_t l){
	return (ReleaseMutex((HANDLE)l)?SLL_NO_ERROR:sll_platform_get_error());
}

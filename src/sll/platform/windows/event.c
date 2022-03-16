#include <windows.h>
#include <sll/common.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_event_handle_t sll_platform_event_create(void){
	return (sll_event_handle_t)CreateEventA(NULL,FALSE,FALSE,NULL);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_event_delete(sll_event_handle_t e){
	return !!CloseHandle((HANDLE)e);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_event_set(sll_event_handle_t e){
	return !!SetEvent((HANDLE)e);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_event_wait(sll_event_handle_t e){
	return (WaitForSingleObject((HANDLE)e,INFINITE)==WAIT_OBJECT_0);
}

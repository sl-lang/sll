#include <windows.h>
#include <sll/_internal/error.h>
#include <sll/common.h>
#include <sll/error.h>
#include <sll/platform/event.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_event_handle_t sll_platform_event_create(sll_error_t* err){
	ERROR_PTR_RESET;
	sll_event_handle_t o=(sll_event_handle_t)CreateEventA(NULL,FALSE,FALSE,NULL);
	if (!o){
		ERROR_PTR_SYSTEM;
	}
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_event_delete(sll_event_handle_t e){
	return (CloseHandle((HANDLE)e)?SLL_NO_ERROR:sll_platform_get_error());
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_event_set(sll_event_handle_t e){
	return (SetEvent((HANDLE)e)?SLL_NO_ERROR:sll_platform_get_error());
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_event_wait(sll_event_handle_t e){
	return (WaitForSingleObject((HANDLE)e,INFINITE)==WAIT_OBJECT_0?SLL_NO_ERROR:sll_platform_get_error());
}

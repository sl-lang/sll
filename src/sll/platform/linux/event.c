#include <sll/_internal/common.h>
#include <sll/_internal/error.h>
#include <sll/_size_types.h>
#include <sll/common.h>
#include <sll/error.h>
#include <sll/platform/event.h>
#include <sll/types.h>
#ifdef __SLL_BUILD_DARWIN
#include <dispatch/dispatch.h>
#else
#include <sys/eventfd.h>
#include <poll.h>
#include <unistd.h>
#endif



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_event_handle_t sll_platform_event_create(sll_error_t* err){
	ERROR_PTR_RESET;
#ifdef __SLL_BUILD_DARWIN
	return (sll_event_handle_t)dispatch_semaphore_create(0);
#else
	int o=eventfd(0,0);
	if (o!=-1){
		return (sll_event_handle_t)PTR(o);
	}
	ERROR_PTR_SYSTEM;
	return NULL;
#endif
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_event_delete(sll_event_handle_t e){
#ifdef __SLL_BUILD_DARWIN
	dispatch_release(*((dispatch_semaphore_t*)(&e)));
	return SLL_NO_ERROR;
#else
	return (close((int)ADDR(e))?sll_platform_get_error():SLL_NO_ERROR);
#endif
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_event_set(sll_event_handle_t e){
#ifdef __SLL_BUILD_DARWIN
	dispatch_semaphore_signal(*((dispatch_semaphore_t*)(&e)));
	return SLL_NO_ERROR;
#else
	__SLL_U64 val=1;
	return (write((int)ADDR(e),&val,sizeof(__SLL_U64))==sizeof(__SLL_U64)?SLL_NO_ERROR:sll_platform_get_error());
#endif
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_event_wait(sll_event_handle_t e){
#ifdef __SLL_BUILD_DARWIN
	return (dispatch_semaphore_wait(*((dispatch_semaphore_t*)(&e)),DISPATCH_TIME_FOREVER)?0:SLL_NO_ERROR);
#else
	__SLL_U64 val;
	return (read((int)ADDR(e),&val,sizeof(__SLL_U64))==sizeof(__SLL_U64)?SLL_NO_ERROR:sll_platform_get_error());
#endif
}

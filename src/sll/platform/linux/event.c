#include <sll/_internal/common.h>
#include <sll/common.h>
#include <sll/types.h>
#ifdef __SLL_BUILD_DARWIN
#include <dispatch/dispatch.h>
#else
#include <sys/eventfd.h>
#include <poll.h>
#include <unistd.h>
#endif



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_event_handle_t sll_platform_event_create(void){
#ifdef __SLL_BUILD_DARWIN
	return (sll_event_handle_t)dispatch_semaphore_create(0);
#else
	return (sll_event_handle_t)PTR(eventfd(0,0));
#endif
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_event_delete(sll_event_handle_t e){
#ifdef __SLL_BUILD_DARWIN
	dispatch_release(*((dispatch_object_t*)(&e)));
	return 1;
#else
	return !close((int)ADDR(e));
#endif
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_event_set(sll_event_handle_t e){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_event_wait(sll_event_handle_t e){
	SLL_UNIMPLEMENTED();
}

#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/common.h>
#include <sll/types.h>



__API_FUNC(thread_create){
	sll_thread_index_t tid=_init_thread_stack(a,b->v,b->l);
	_scheduler_queue_thread(tid);
	return tid;
}

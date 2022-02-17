#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/common.h>
#include <sll/scheduler.h>
#include <sll/types.h>



__API_FUNC(thread_create){
	return sll_create_thread(a,b->v,b->l);
}



__API_FUNC(thread_create_lock){
	return sll_create_lock();
}



__API_FUNC(thread_release_lock){
	if (a<0||a>SLL_MAX_LOCK_INDEX){
		return 0;
	}
	return sll_release_lock((sll_lock_index_t)a);
}

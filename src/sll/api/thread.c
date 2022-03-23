#include <sll/_internal/api.h>
#include <sll/_internal/thread.h>
#include <sll/api.h>
#include <sll/array.h>
#include <sll/barrier.h>
#include <sll/common.h>
#include <sll/lock.h>
#include <sll/object.h>
#include <sll/semaphore.h>
#include <sll/static_object.h>
#include <sll/thread.h>
#include <sll/types.h>



__API_FUNC(thread_create){
	sll_thread_index_t o=sll_thread_create(a,b->v,b->l);
	return (!sll_thread_start(o)?-1:o);
}



__API_FUNC(thread_create_barrier){
	return sll_barrier_create();
}



__API_FUNC(thread_create_lock){
	return sll_lock_create();
}



__API_FUNC(thread_create_semaphore){
	return sll_semaphore_create((a<0?0:(sll_semaphore_counter_t)a));
}



__API_FUNC(thread_get_internal_data){
	thread_data_t* thr=_thread_get((a<0?0:(sll_thread_index_t)a));
	sll_new_object_array(SLL_CHAR("hh"),out,(thr?thr->ii:0),(thr?thr->si:0));
}



__API_FUNC(thread_increase_barrier){
	return sll_barrier_increase((a<0?0:(sll_barrier_index_t)a));
}



__API_FUNC(thread_release_lock){
	if (a<0||a>SLL_MAX_LOCK_INDEX){
		return 0;
	}
	return sll_lock_release((sll_lock_index_t)a);
}



__API_FUNC(thread_release_semaphore){
	if (a<0||a>SLL_MAX_SEMAPHORE_INDEX){
		return 0;
	}
	return sll_semaphore_release((sll_semaphore_index_t)a);
}



__API_FUNC(thread_reset_barrier){
	return sll_barrier_reset((a<0?0:(sll_barrier_index_t)a));
}



__API_FUNC(thread_restart){
	return sll_thread_restart((a<0?0:(sll_thread_index_t)a));
}



__API_FUNC(thread_suspend){
	return sll_thread_suspend((a<0?0:(sll_thread_index_t)a));
}

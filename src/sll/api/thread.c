#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/scheduler.h>
#include <sll/static_object.h>
#include <sll/types.h>



__API_FUNC(thread_create){
	sll_thread_index_t o=sll_create_thread(a,b->v,b->l);
	return (!sll_start_thread(o)?-1:o);
}



__API_FUNC(thread_create_barrier){
	return sll_create_barrier();
}



__API_FUNC(thread_create_lock){
	return sll_create_lock();
}



__API_FUNC(thread_create_semaphore){
	return sll_create_semaphore((a<0?0:(sll_semaphore_counter_t)a));
}



__API_FUNC(thread_get_internal_data){
	thread_data_t* thr=_scheduler_get_thread((a<0?0:(sll_thread_index_t)a));
	sll_array_create(2,out);
	if (thr){
		out->v[0]=sll_int_to_object(thr->ii);
		out->v[1]=sll_int_to_object(thr->si);
	}
	else{
		out->v[0]=SLL_ACQUIRE_STATIC_INT(0);
		out->v[1]=SLL_ACQUIRE_STATIC_INT(0);
	}
}



__API_FUNC(thread_increase_barrier){
	return sll_increase_barrier((a<0?0:(sll_barrier_index_t)a));
}



__API_FUNC(thread_release_lock){
	if (a<0||a>SLL_MAX_LOCK_INDEX){
		return 0;
	}
	return sll_release_lock((sll_lock_index_t)a);
}



__API_FUNC(thread_release_semaphore){
	if (a<0||a>SLL_MAX_SEMAPHORE_INDEX){
		return 0;
	}
	return sll_release_semaphore((sll_semaphore_index_t)a);
}



__API_FUNC(thread_reset_barrier){
	return sll_reset_barrier((a<0?0:(sll_barrier_index_t)a));
}



__API_FUNC(thread_restart){
	return sll_restart_thread((a<0?0:(sll_thread_index_t)a));
}



__API_FUNC(thread_suspend){
	return sll_suspend_thread((a<0?0:(sll_thread_index_t)a));
}

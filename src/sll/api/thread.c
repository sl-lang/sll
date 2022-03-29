#include <sll/_internal/api.h>
#include <sll/_internal/thread.h>
#include <sll/api.h>
#include <sll/array.h>
#include <sll/barrier.h>
#include <sll/common.h>
#include <sll/lock.h>
#include <sll/new_object.h>
#include <sll/sandbox.h>
#include <sll/semaphore.h>
#include <sll/static_object.h>
#include <sll/thread.h>
#include <sll/types.h>



__API_FUNC(thread_create){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)){
		return -1;
	}
	sll_thread_index_t o=sll_thread_create(a,b->v,b->l);
	return (!sll_thread_start(o)?-1:o);
}



__API_FUNC(thread_create_barrier){
	return (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)?-1:sll_barrier_create());
}



__API_FUNC(thread_create_lock){
	return (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)?-1:sll_lock_create());
}



__API_FUNC(thread_create_semaphore){
	return (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)?-1:sll_semaphore_create((a<0?0:(sll_semaphore_counter_t)a)));
}



__API_FUNC(thread_delete){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)||a<0||a>SLL_MAX_THREAD_INDEX){
		return 0;
	}
	return sll_thread_delete((sll_thread_index_t)a);
}



__API_FUNC(thread_delete_barrier){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)||a<0||a>SLL_MAX_BARRIER_INDEX){
		return 0;
	}
	return sll_barrier_delete((sll_barrier_index_t)a);
}



__API_FUNC(thread_delete_lock){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)||a<0||a>SLL_MAX_LOCK_INDEX){
		return 0;
	}
	return sll_lock_delete((sll_lock_index_t)a);
}



__API_FUNC(thread_delete_semaphore){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)||a<0||a>SLL_MAX_SEMAPHORE_INDEX){
		return 0;
	}
	return sll_semaphore_delete((sll_semaphore_index_t)a);
}



__API_FUNC(thread_get_internal_data){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)||a<0||a>SLL_MAX_THREAD_INDEX){
		goto _error;
	}
	thread_data_t* thr=_thread_get((sll_thread_index_t)a);
	if (thr){
		sll_new_object_array(SLL_CHAR("hh"),out,thr->ii,thr->si);
		return;
	}
_error:
	sll_new_object_array(SLL_CHAR("00"),out);
}



__API_FUNC(thread_increase_barrier){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)||a<0||a>SLL_MAX_BARRIER_INDEX){
		return 0;
	}
	return sll_barrier_increase((sll_barrier_index_t)a);
}



__API_FUNC(thread_release_lock){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)||a<0||a>SLL_MAX_LOCK_INDEX){
		return 0;
	}
	return sll_lock_release((sll_lock_index_t)a);
}



__API_FUNC(thread_release_semaphore){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)||a<0||a>SLL_MAX_SEMAPHORE_INDEX){
		return 0;
	}
	return sll_semaphore_release((sll_semaphore_index_t)a);
}



__API_FUNC(thread_reset_barrier){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)||a<0||a>SLL_MAX_BARRIER_INDEX){
		return 0;
	}
	return sll_barrier_reset((sll_barrier_index_t)a);
}



__API_FUNC(thread_restart){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)||a<0||a>SLL_MAX_THREAD_INDEX){
		return 0;
	}
	return sll_thread_restart((sll_thread_index_t)a);
}



__API_FUNC(thread_suspend){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_THREADS)||a<0||a>SLL_MAX_THREAD_INDEX){
		return 0;
	}
	return sll_thread_suspend((sll_thread_index_t)a);
}

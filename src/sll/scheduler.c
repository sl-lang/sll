#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/memory.h>
#include <sll/platform.h>
#include <sll/scheduler.h>
#include <sll/types.h>
#include <sll/vm.h>



static thread_data_t** _scheduler_thread;
static sll_thread_index_t _scheduler_thread_next;
static thread_list_length_t _scheduler_thread_len;
static sll_thread_index_t* _scheduler_queue;
static queue_length_t _scheduler_queue_idx;
static queue_length_t _scheduler_queue_len;
static barrier_t* _scheduler_barrier;
static sll_barrier_index_t _scheduler_barrier_next;
static barrier_list_length_t _scheduler_barrier_len;
static lock_t* _scheduler_lock;
static sll_lock_index_t _scheduler_lock_next;
static lock_list_length_t _scheduler_lock_len;
static semaphore_t* _scheduler_semaphore;
static sll_semaphore_index_t _scheduler_semaphore_next;
static semaphore_list_length_t _scheduler_semaphore_len;
static void* _scheduler_allocator_cache_pool[THREAD_ALLOCATOR_CACHE_POOL_SIZE];
static sll_array_length_t _scheduler_allocator_cache_pool_len;



thread_data_t* _scheduler_current_thread;



__SLL_EXTERNAL sll_thread_index_t sll_current_thread_index=0;



static void _queue_barrier(barrier_t* b){
	if (b->first==SLL_UNKNOWN_THREAD_INDEX){
		return;
	}
	SLL_UNIMPLEMENTED();
}



void _scheduler_deinit(void){
	while (_scheduler_thread_len){
		_scheduler_thread_len--;
		thread_data_t* thr=*(_scheduler_thread+_scheduler_thread_len);
		if (THREAD_IS_UNUSED(thr)){
			continue;
		}
		if (thr->st==THREAD_STATE_TERMINATED){
			SLL_RELEASE(thr->ret);
		}
		sll_platform_free_page(thr,THREAD_SIZE);
	}
	while (_scheduler_allocator_cache_pool_len){
		_scheduler_allocator_cache_pool_len--;
		sll_platform_free_page(_scheduler_allocator_cache_pool[_scheduler_allocator_cache_pool_len],THREAD_SIZE);
	}
	sll_deallocate(_scheduler_thread);
	sll_deallocate(_scheduler_queue);
	sll_deallocate(_scheduler_barrier);
	sll_deallocate(_scheduler_lock);
	sll_deallocate(_scheduler_semaphore);
	_scheduler_current_thread=NULL;
	sll_current_thread_index=SLL_UNKNOWN_THREAD_INDEX;
}



thread_data_t* _scheduler_get_thread(sll_thread_index_t t){
	return (t>=_scheduler_thread_len||THREAD_IS_UNUSED(*(_scheduler_thread+t))?NULL:*(_scheduler_thread+t));
}



void _scheduler_init(void){
	_scheduler_thread=NULL;
	_scheduler_thread_next=SLL_UNKNOWN_THREAD_INDEX;
	_scheduler_thread_len=0;
	_scheduler_queue=NULL;
	_scheduler_queue_idx=0;
	_scheduler_queue_len=0;
	_scheduler_barrier=NULL;
	_scheduler_barrier_next=THREAD_BARRIER_UNUSED;
	_scheduler_barrier_len=0;
	_scheduler_lock=NULL;
	_scheduler_lock_next=THREAD_LOCK_UNUSED;
	_scheduler_lock_len=0;
	_scheduler_semaphore=NULL;
	_scheduler_semaphore_next=THREAD_SEMAPHORE_UNUSED;
	_scheduler_semaphore_len=0;
	_scheduler_allocator_cache_pool_len=0;
	_scheduler_current_thread=NULL;
	sll_current_thread_index=SLL_UNKNOWN_THREAD_INDEX;
}



sll_thread_index_t _scheduler_new_thread(void){
	sll_thread_index_t o=_scheduler_thread_next;
	if (o==SLL_UNKNOWN_THREAD_INDEX){
		o=_scheduler_thread_len;
		_scheduler_thread_len++;
		_scheduler_thread=sll_reallocate(_scheduler_thread,_scheduler_thread_len*sizeof(thread_data_t*));
	}
	else{
		_scheduler_thread_next=THREAD_GET_NEXT_UNUSED(*(_scheduler_thread+o));
	}
	void* ptr=NULL;
	if (_scheduler_allocator_cache_pool_len){
		_scheduler_allocator_cache_pool_len--;
		ptr=_scheduler_allocator_cache_pool[_scheduler_allocator_cache_pool_len];
	}
	else{
		ptr=sll_platform_allocate_page(THREAD_SIZE,0);
	}
	thread_data_t* n=ptr;
	n->stack=PTR(ADDR(ptr)+sizeof(thread_data_t)+sll_current_vm_config->c_st_sz*sizeof(sll_call_stack_frame_t));
	n->ii=0;
	n->si=0;
	n->wait=SLL_UNKNOWN_THREAD_INDEX;
	n->ret=NULL;
	n->c_st.dt=PTR(ADDR(ptr)+sizeof(thread_data_t));
	n->c_st.l=0;
	n->tm=THREAD_SCHEDULER_INSTRUCTION_COUNT;
	n->st=THREAD_STATE_INITIALIZED;
	*(_scheduler_thread+o)=n;
	if (sll_current_thread_index==SLL_UNKNOWN_THREAD_INDEX){
		_scheduler_current_thread=n;
		sll_current_thread_index=o;
		n->st=THREAD_STATE_RUNNING;
	}
	return o;
}



void _scheduler_queue_next(void){
	_scheduler_current_thread->tm=THREAD_SCHEDULER_INSTRUCTION_COUNT;
	if (!_scheduler_queue_len){
		return;
	}
	_scheduler_current_thread->st=THREAD_STATE_QUEUED;
	sll_thread_index_t tmp=*(_scheduler_queue+_scheduler_queue_idx);
	*(_scheduler_queue+_scheduler_queue_idx)=sll_current_thread_index;
	if (!_scheduler_queue_idx){
		_scheduler_queue_idx=_scheduler_queue_len;
	}
	_scheduler_queue_idx--;
	sll_current_thread_index=tmp;
	_scheduler_current_thread=*(_scheduler_thread+tmp);
	_scheduler_current_thread->st=THREAD_STATE_RUNNING;
}



sll_thread_index_t _scheduler_queue_pop(void){
	if (!_scheduler_queue_len){
		return SLL_UNKNOWN_THREAD_INDEX;
	}
	sll_thread_index_t o=*(_scheduler_queue+_scheduler_queue_idx);
	for (queue_length_t i=_scheduler_queue_idx+1;i<_scheduler_queue_len;i++){
		*(_scheduler_queue+i-1)=*(_scheduler_queue+i);
	}
	_scheduler_queue_len--;
	_scheduler_queue=sll_reallocate(_scheduler_queue,_scheduler_queue_len*sizeof(sll_thread_index_t));
	if (!_scheduler_queue_len){
		_scheduler_queue_idx=0;
	}
	else{
		if (!_scheduler_queue_idx){
			_scheduler_queue_idx=_scheduler_queue_len;
		}
		_scheduler_queue_idx--;
	}
	(*(_scheduler_thread+o))->st=THREAD_STATE_UNDEFINED;
	return o;
}



void _scheduler_queue_thread(sll_thread_index_t t){
	_scheduler_queue_len++;
	_scheduler_queue=sll_reallocate(_scheduler_queue,_scheduler_queue_len*sizeof(sll_thread_index_t));
	*(_scheduler_queue+_scheduler_queue_len-1)=t;
}



void _scheduler_set_thread(sll_thread_index_t t){
	if (sll_current_thread_index==t){
		return;
	}
	if (sll_current_thread_index!=SLL_UNKNOWN_THREAD_INDEX){
		_scheduler_queue_thread(sll_current_thread_index);
		sll_current_thread_index=SLL_UNKNOWN_THREAD_INDEX;
	}
	if ((*(_scheduler_thread+t))->st==THREAD_STATE_TERMINATED){
		sll_current_thread_index=SLL_UNKNOWN_THREAD_INDEX;
		return;
	}
	sll_current_thread_index=t;
	_scheduler_current_thread=*(_scheduler_thread+t);
	if (_scheduler_current_thread->st!=THREAD_STATE_INITIALIZED&&_scheduler_current_thread->st!=THREAD_STATE_RUNNING&&_scheduler_current_thread->st!=THREAD_STATE_UNDEFINED){
		SLL_UNIMPLEMENTED();
	}
	_scheduler_current_thread->st=THREAD_STATE_RUNNING;
}



void _scheduler_terminate_thread(sll_object_t* ret){
	SLL_ASSERT(_scheduler_current_thread->st==THREAD_STATE_RUNNING);
	SLL_ACQUIRE(ret);
	_scheduler_current_thread->ret=ret;
	_scheduler_current_thread->st=THREAD_STATE_TERMINATED;
	sll_current_thread_index=_scheduler_current_thread->wait;
	if (sll_current_thread_index==SLL_UNKNOWN_THREAD_INDEX){
		sll_current_thread_index=_scheduler_queue_pop();
		if (sll_current_thread_index!=SLL_UNKNOWN_THREAD_INDEX){
			_scheduler_current_thread=*(_scheduler_thread+sll_current_thread_index);
		}
		return;
	}
	_scheduler_current_thread=*(_scheduler_thread+sll_current_thread_index);
	while (_scheduler_current_thread->nxt!=SLL_UNKNOWN_THREAD_INDEX){
		_scheduler_current_thread->st=THREAD_STATE_QUEUED;
		_scheduler_queue_thread(sll_current_thread_index);
		sll_current_thread_index=_scheduler_current_thread->nxt;
		_scheduler_current_thread=*(_scheduler_thread+sll_current_thread_index);
	}
	_scheduler_current_thread->st=THREAD_STATE_RUNNING;
}



sll_bool_t _scheduler_wait_barrier(sll_integer_t w,sll_integer_t v,sll_bool_t g){
	if (w<0||w>=_scheduler_barrier_len||(_scheduler_barrier+w)->count==THREAD_BARRIER_UNUSED){
		return 0;
	}
	if ((_scheduler_barrier+w)->count==v||(g&&(_scheduler_barrier+w)->count>v)){
		return 0;
	}
	if ((_scheduler_barrier+w)->first==SLL_UNKNOWN_THREAD_INDEX){
		(_scheduler_barrier+w)->first=sll_current_thread_index;
	}
	else{
		(_scheduler_barrier+w)->last->nxt=sll_current_thread_index;
	}
	(_scheduler_barrier+w)->last=_scheduler_current_thread;
	_scheduler_current_thread->nxt=SLL_UNKNOWN_THREAD_INDEX;
	_scheduler_current_thread->st=THREAD_STATE_WAIT_BARRIER;
	sll_current_thread_index=SLL_UNKNOWN_THREAD_INDEX;
	return 1;
}



sll_bool_t _scheduler_wait_lock(sll_integer_t w){
	if (w<0||w>=_scheduler_lock_len||(_scheduler_lock+w)->lock==THREAD_LOCK_UNUSED){
		return 0;
	}
	SLL_ASSERT((_scheduler_lock+w)->lock!=sll_current_thread_index);
	if ((_scheduler_lock+w)->lock==SLL_UNKNOWN_THREAD_INDEX){
		(_scheduler_lock+w)->lock=sll_current_thread_index;
		(_scheduler_lock+w)->first=SLL_UNKNOWN_THREAD_INDEX;
		return 0;
	}
	if ((_scheduler_lock+w)->first==SLL_UNKNOWN_THREAD_INDEX){
		(_scheduler_lock+w)->first=sll_current_thread_index;
	}
	else{
		(_scheduler_lock+w)->last->nxt=sll_current_thread_index;
	}
	(_scheduler_lock+w)->last=_scheduler_current_thread;
	_scheduler_current_thread->nxt=SLL_UNKNOWN_THREAD_INDEX;
	_scheduler_current_thread->st=THREAD_STATE_WAIT_LOCK;
	sll_current_thread_index=SLL_UNKNOWN_THREAD_INDEX;
	return 1;
}



sll_bool_t _scheduler_wait_semaphore(sll_integer_t w){
	if (w<0||w>=_scheduler_semaphore_len||(_scheduler_semaphore+w)->count==THREAD_SEMAPHORE_UNUSED){
		return 0;
	}
	if ((_scheduler_semaphore+w)->count){
		(_scheduler_semaphore+w)->count--;
		(_scheduler_semaphore+w)->first=SLL_UNKNOWN_THREAD_INDEX;
		return 0;
	}
	if ((_scheduler_semaphore+w)->first==SLL_UNKNOWN_THREAD_INDEX){
		(_scheduler_semaphore+w)->first=sll_current_thread_index;
	}
	else{
		(_scheduler_semaphore+w)->last->nxt=sll_current_thread_index;
	}
	(_scheduler_semaphore+w)->last=_scheduler_current_thread;
	_scheduler_current_thread->nxt=SLL_UNKNOWN_THREAD_INDEX;
	_scheduler_current_thread->st=THREAD_STATE_WAIT_SEMAPHORE;
	sll_current_thread_index=SLL_UNKNOWN_THREAD_INDEX;
	return 1;
}



sll_bool_t _scheduler_wait_thread(sll_integer_t w){
	if (w<0||w>=_scheduler_thread_len||!*(_scheduler_thread+w)||w==sll_current_thread_index||(*(_scheduler_thread+w))->ret){
		return 0;
	}
	_scheduler_current_thread->tm=THREAD_SCHEDULER_INSTRUCTION_COUNT;
	thread_data_t* thr=*(_scheduler_thread+w);
	_scheduler_current_thread->nxt=thr->wait;
	_scheduler_current_thread->st=THREAD_STATE_WAIT_THREAD;
	thr->wait=sll_current_thread_index;
	sll_current_thread_index=SLL_UNKNOWN_THREAD_INDEX;
	return 1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_barrier_index_t sll_create_barrier(void){
	sll_barrier_index_t o=_scheduler_barrier_next;
	if (o==THREAD_BARRIER_UNUSED){
		o=_scheduler_barrier_len;
		_scheduler_barrier_len++;
		_scheduler_barrier=sll_reallocate(_scheduler_barrier,_scheduler_barrier_len*sizeof(barrier_t));
	}
	else{
		_scheduler_barrier_next=THREAD_BARRIER_GET_NEXT_ID(_scheduler_barrier+o);
	}
	(_scheduler_barrier+o)->count=0;
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_lock_index_t sll_create_lock(void){
	sll_lock_index_t o=_scheduler_lock_next;
	if (o==THREAD_LOCK_UNUSED){
		o=_scheduler_lock_len;
		_scheduler_lock_len++;
		_scheduler_lock=sll_reallocate(_scheduler_lock,_scheduler_lock_len*sizeof(lock_t));
	}
	else{
		_scheduler_lock_next=THREAD_LOCK_GET_NEXT_ID(_scheduler_lock+o);
	}
	(_scheduler_lock+o)->lock=SLL_UNKNOWN_THREAD_INDEX;
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_semaphore_index_t sll_create_semaphore(sll_semaphore_counter_t c){
	sll_semaphore_index_t o=_scheduler_semaphore_next;
	if (o==THREAD_SEMAPHORE_UNUSED){
		o=_scheduler_semaphore_len;
		_scheduler_semaphore_len++;
		_scheduler_semaphore=sll_reallocate(_scheduler_semaphore,_scheduler_semaphore_len*sizeof(semaphore_t));
	}
	else{
		_scheduler_semaphore_next=THREAD_SEMAPHORE_GET_NEXT_ID(_scheduler_semaphore+o);
	}
	(_scheduler_semaphore+o)->count=c;
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_thread_index_t sll_create_thread(sll_integer_t fn,sll_object_t*const* al,sll_arg_count_t all){
	if (fn<0){
		SLL_UNIMPLEMENTED();
	}
	if (fn&&fn<=sll_current_runtime_data->a_dt->ft.l){
		sll_thread_index_t o=_scheduler_new_thread();
		thread_data_t* thr=_scheduler_get_thread(o);
		for (;thr->si<all;thr->si++){
			*(thr->stack+thr->si)=*(al+thr->si);
			SLL_ACQUIRE(*(al+thr->si));
		}
		_call_function(thr,(sll_function_index_t)(fn-1),all);
		return o;
	}
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_delete_barrier(sll_barrier_index_t b){
	if (b>=_scheduler_barrier_len||(_scheduler_barrier+b)->count==THREAD_BARRIER_UNUSED){
		return 0;
	}
	(_scheduler_barrier+b)->count=THREAD_BARRIER_UNUSED;
	THREAD_BARRIER_SET_NEXT_ID(_scheduler_barrier+b,_scheduler_barrier_next);
	_scheduler_barrier_next=b;
	return 1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_delete_lock(sll_lock_index_t l){
	if (l>=_scheduler_lock_len||(_scheduler_lock+l)->lock==THREAD_LOCK_UNUSED){
		return 0;
	}
	SLL_ASSERT((_scheduler_lock+l)->lock==SLL_UNKNOWN_THREAD_INDEX);
	(_scheduler_lock+l)->lock=THREAD_LOCK_UNUSED;
	THREAD_LOCK_SET_NEXT_ID(_scheduler_lock+l,_scheduler_lock_next);
	_scheduler_lock_next=l;
	return 1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_delete_semaphore(sll_semaphore_index_t s){
	if (s>=_scheduler_semaphore_len||(_scheduler_semaphore+s)->count==THREAD_SEMAPHORE_UNUSED){
		return 0;
	}
	(_scheduler_semaphore+s)->count=THREAD_SEMAPHORE_UNUSED;
	THREAD_SEMAPHORE_SET_NEXT_ID(_scheduler_semaphore+s,_scheduler_semaphore_next);
	_scheduler_semaphore_next=s;
	return 1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_delete_thread(sll_thread_index_t t){
	if (t>=_scheduler_thread_len){
		return 0;
	}
	thread_data_t* thr=*(_scheduler_thread+t);
	if (THREAD_IS_UNUSED(thr)){
		return 0;
	}
	SLL_ASSERT(thr->st==THREAD_STATE_TERMINATED);
	*(_scheduler_thread+t)=THREAD_NEXT_UNUSED(_scheduler_thread_next);
	_scheduler_thread_next=t;
	SLL_RELEASE(thr->ret);
	if (_scheduler_allocator_cache_pool_len<THREAD_ALLOCATOR_CACHE_POOL_SIZE){
		_scheduler_allocator_cache_pool[_scheduler_allocator_cache_pool_len]=thr;
		_scheduler_allocator_cache_pool_len++;
	}
	else{
		sll_platform_free_page(thr,THREAD_SIZE);
	}
	return 1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_barrier_counter_t sll_increase_barrier(sll_barrier_index_t b){
	if (b>=_scheduler_barrier_len||(_scheduler_barrier+b)->count==THREAD_BARRIER_UNUSED){
		return 0;
	}
	(_scheduler_barrier+b)->count++;
	_queue_barrier(_scheduler_barrier+b);
	return (_scheduler_barrier+b)->count;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_release_lock(sll_lock_index_t l){
	if (l>=_scheduler_lock_len||(_scheduler_lock+l)->lock==THREAD_LOCK_UNUSED||(_scheduler_lock+l)->lock==SLL_UNKNOWN_THREAD_INDEX){
		return 0;
	}
	(_scheduler_lock+l)->lock=(_scheduler_lock+l)->first;
	if ((_scheduler_lock+l)->first==SLL_UNKNOWN_THREAD_INDEX){
		return 1;
	}
	thread_data_t* thr=*(_scheduler_thread+(_scheduler_lock+l)->lock);
	(_scheduler_lock+l)->first=thr->nxt;
	thr->st=THREAD_STATE_QUEUED;
	_scheduler_queue_thread((_scheduler_lock+l)->lock);
	return 1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_release_semaphore(sll_semaphore_index_t l){
	if (l>=_scheduler_semaphore_len||(_scheduler_semaphore+l)->count==THREAD_SEMAPHORE_UNUSED){
		return 0;
	}
	if ((_scheduler_semaphore+l)->first==SLL_UNKNOWN_THREAD_INDEX){
		(_scheduler_semaphore+l)->count++;
		return 1;
	}
	thread_data_t* thr=*(_scheduler_thread+(_scheduler_semaphore+l)->first);
	thr->st=THREAD_STATE_QUEUED;
	_scheduler_queue_thread((_scheduler_semaphore+l)->first);
	(_scheduler_semaphore+l)->first=thr->nxt;
	return 1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_reset_barrier(sll_barrier_index_t b){
	if (b>=_scheduler_barrier_len||(_scheduler_barrier+b)->count==THREAD_BARRIER_UNUSED){
		return 0;
	}
	if ((_scheduler_barrier+b)->count){
		(_scheduler_barrier+b)->count=0;
		_queue_barrier(_scheduler_barrier+b);
	}
	return 1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_start_thread(sll_thread_index_t t){
	if (t>=_scheduler_thread_len){
		return 0;
	}
	thread_data_t* thr=*(_scheduler_thread+t);
	if (THREAD_IS_UNUSED(thr)||thr->st!=THREAD_STATE_INITIALIZED){
		return 0;
	}
	thr->st=THREAD_STATE_QUEUED;
	_scheduler_queue_thread(t);
	return 1;
}

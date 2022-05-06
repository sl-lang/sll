#include <sll/_internal/atexit.h>
#include <sll/_internal/gc.h>
#include <sll/_internal/sandbox.h>
#include <sll/_internal/scheduler.h>
#include <sll/_internal/vm.h>
#include <sll/audit.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/memory.h>
#include <sll/platform/lock.h>
#include <sll/platform/memory.h>
#include <sll/scheduler.h>
#include <sll/static_object.h>
#include <sll/thread.h>
#include <sll/types.h>
#include <sll/vm.h>



static sll_lock_handle_t _thread_lock;
static sll_thread_index_t _thread_next;
static thread_list_length_t _thread_len;
static void* _scheduler_allocator_cache_pool[THREAD_ALLOCATOR_CACHE_POOL_SIZE];
static sll_array_length_t _scheduler_allocator_cache_pool_len;



volatile thread_list_length_t _thread_active_count;
thread_data_t** _thread_data;



void _thread_deinit(void){
	while (_thread_len){
		_thread_len--;
		thread_data_t* thr=*(_thread_data+_thread_len);
		if (THREAD_IS_UNUSED(thr)){
			continue;
		}
		if (thr->st==THREAD_STATE_TERMINATED){
			SLL_RELEASE(thr->ret);
		}
		SLL_CRITICAL_ERROR(sll_platform_free_page(thr,THREAD_SIZE));
	}
	while (_scheduler_allocator_cache_pool_len){
		_scheduler_allocator_cache_pool_len--;
		SLL_CRITICAL_ERROR(sll_platform_free_page(_scheduler_allocator_cache_pool[_scheduler_allocator_cache_pool_len],THREAD_SIZE));
	}
	sll_deallocate(_thread_data);
	SLL_CRITICAL(sll_platform_lock_delete(_thread_lock));
}



thread_data_t* _thread_get(sll_thread_index_t t){
	return (t>=_thread_len||THREAD_IS_UNUSED(*(_thread_data+t))?NULL:*(_thread_data+t));
}



void _thread_init(void){
	_thread_lock=sll_platform_lock_create(NULL);
	_thread_data=NULL;
	_thread_active_count=0;
	_thread_next=SLL_UNKNOWN_THREAD_INDEX;
	_thread_len=0;
	_scheduler_allocator_cache_pool_len=0;
}



sll_thread_index_t _thread_new(void){
	SLL_CRITICAL_ERROR(sll_platform_lock_acquire(_thread_lock));
	sll_thread_index_t o=_thread_next;
	if (o==SLL_UNKNOWN_THREAD_INDEX){
		o=_thread_len;
		_thread_len++;
		_thread_data=sll_reallocate(_thread_data,_thread_len*sizeof(thread_data_t*));
	}
	else{
		_thread_next=THREAD_GET_NEXT_UNUSED(*(_thread_data+o));
	}
	void* ptr=NULL;
	if (_scheduler_allocator_cache_pool_len){
		_scheduler_allocator_cache_pool_len--;
		ptr=_scheduler_allocator_cache_pool[_scheduler_allocator_cache_pool_len];
	}
	else{
		ptr=sll_platform_allocate_page(THREAD_SIZE,0,NULL);
	}
	thread_data_t* n=ptr;
	n->stack=PTR(ADDR(ptr)+sizeof(thread_data_t)+sll_current_vm_config->c_st_sz*sizeof(sll_call_stack_frame_t)+sll_current_runtime_data->a_dt->tls_vc*sizeof(sll_object_t*));
	n->tls=PTR(ADDR(ptr)+sizeof(thread_data_t)+sll_current_vm_config->c_st_sz*sizeof(sll_call_stack_frame_t));
	sll_static_int[0]->rc+=sll_current_runtime_data->a_dt->tls_vc;
	for (sll_variable_index_t i=0;i<sll_current_runtime_data->a_dt->tls_vc;i++){
		*(n->tls+i)=sll_static_int[0];
	}
	n->ii=0;
	n->si=0;
	n->wait=SLL_UNKNOWN_THREAD_INDEX;
	n->ret=NULL;
	n->c_st.dt=PTR(ADDR(ptr)+sizeof(thread_data_t));
	n->c_st.l=0;
	n->sandbox=(_scheduler_current_thread_index==SLL_UNKNOWN_THREAD_INDEX?_sandbox_flags:_scheduler_current_thread->sandbox);
	n->tm=THREAD_SCHEDULER_INSTRUCTION_COUNT;
	n->st=THREAD_STATE_INITIALIZED;
	n->flags=0;
	n->_last_ai=NULL;
	*(_thread_data+o)=n;
	_thread_active_count++;
	SLL_CRITICAL(sll_platform_lock_release(_thread_lock));
	return o;
}



void _thread_terminate(sll_object_t* ret){
	SLL_ASSERT(_scheduler_current_thread->st==THREAD_STATE_RUNNING&&!(_scheduler_current_thread->flags&THREAD_FLAG_SUSPENDED));
	SLL_ACQUIRE(ret);
	sll_thread_index_t tid=_scheduler_current_thread_index;
	sll_bool_t del=!!(_scheduler_current_thread->flags&THREAD_FLAG_DELETE);
	_scheduler_current_thread->ret=ret;
	_scheduler_current_thread->st=THREAD_STATE_TERMINATED;
	_scheduler_current_thread_index=_scheduler_current_thread->wait;
	if (_scheduler_current_thread_index!=SLL_UNKNOWN_THREAD_INDEX){
		_scheduler_current_thread=*(_thread_data+_scheduler_current_thread_index);
		while (_scheduler_current_thread->nxt!=SLL_UNKNOWN_THREAD_INDEX){
			_scheduler_current_thread->st=THREAD_STATE_QUEUED;
			if (_scheduler_current_thread->flags&THREAD_FLAG_SUSPENDED){
				SLL_UNIMPLEMENTED();
			}
			_scheduler_queue_thread(_scheduler_current_thread_index);
			_scheduler_current_thread_index=_scheduler_current_thread->nxt;
			_scheduler_current_thread=*(_thread_data+_scheduler_current_thread_index);
		}
		_scheduler_current_thread->st=THREAD_STATE_RUNNING;
	}
	if (!(_scheduler_current_thread->flags&THREAD_FLAG_NO_AUDIT_TERMINATE)){
		sll_audit(SLL_CHAR("sll.thread.return"),SLL_CHAR("u"),tid);
	}
	SLL_CRITICAL_ERROR(sll_platform_lock_acquire(_thread_lock));
	if (_thread_active_count==1){
		SLL_CRITICAL(sll_platform_lock_release(_thread_lock));
		sll_audit(SLL_CHAR("sll.vm.shutdown"),SLL_CHAR(""));
		_atexit_execute();
		SLL_CRITICAL_ERROR(sll_platform_lock_acquire(_thread_lock));
	}
	_thread_active_count--;
	SLL_CRITICAL(sll_platform_lock_release(_thread_lock));
	if (del){
		SLL_CRITICAL(sll_thread_delete(tid));
	}
}



sll_bool_t _thread_wait(sll_integer_t w){
	if (w<0||w>=_thread_len||!*(_thread_data+w)||w==_scheduler_current_thread_index||(*(_thread_data+w))->ret){
		return 0;
	}
	_scheduler_current_thread->tm=THREAD_SCHEDULER_INSTRUCTION_COUNT;
	thread_data_t* thr=*(_thread_data+w);
	_scheduler_current_thread->nxt=thr->wait;
	_scheduler_current_thread->st=THREAD_STATE_WAIT_THREAD;
	thr->wait=_scheduler_current_thread_index;
	_scheduler_current_thread_index=SLL_UNKNOWN_THREAD_INDEX;
	return 1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_thread_index_t sll_thread_create(sll_integer_t fn,sll_object_t*const* al,sll_arg_count_t all){
	if (fn<0){
		SLL_UNIMPLEMENTED();
	}
	if (fn&&fn<=sll_current_runtime_data->a_dt->ft.l){
		sll_thread_index_t o=_thread_new();
		thread_data_t* thr=*(_thread_data+o);
		for (;thr->si<all;thr->si++){
			*(thr->stack+thr->si)=*(al+thr->si);
			SLL_ACQUIRE(*(al+thr->si));
		}
		_call_function(thr,(sll_function_index_t)(fn-1),all,0);
		return o;
	}
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_thread_delete(sll_thread_index_t t){
	if (t>=_thread_len){
		return 0;
	}
	thread_data_t* thr=*(_thread_data+t);
	if (THREAD_IS_UNUSED(thr)){
		return 0;
	}
	SLL_CRITICAL_ERROR(sll_platform_lock_acquire(_thread_lock));
	if (thr->st!=THREAD_STATE_TERMINATED){
		thr->flags|=THREAD_FLAG_DELETE;
		SLL_CRITICAL(sll_platform_lock_release(_thread_lock));
		return 1;
	}
	*(_thread_data+t)=THREAD_NEXT_UNUSED(_thread_next);
	_thread_next=t;
	SLL_RELEASE(thr->ret);
	for (sll_variable_index_t i=0;i<sll_current_runtime_data->a_dt->tls_vc;i++){
		SLL_RELEASE(*(thr->tls+i));
	}
	SLL_ASSERT(!thr->si);
	if (_scheduler_allocator_cache_pool_len<THREAD_ALLOCATOR_CACHE_POOL_SIZE){
		_scheduler_allocator_cache_pool[_scheduler_allocator_cache_pool_len]=thr;
		_scheduler_allocator_cache_pool_len++;
	}
	else{
		SLL_CRITICAL_ERROR(sll_platform_free_page(thr,THREAD_SIZE));
	}
	SLL_CRITICAL(sll_platform_lock_release(_thread_lock));
	return 1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT const sll_call_stack_t* sll_thread_get_call_stack(sll_thread_index_t t){
	if (t>=_thread_len){
		return NULL;
	}
	thread_data_t* thr=*(_thread_data+t);
	if (THREAD_IS_UNUSED(thr)){
		return NULL;
	}
	return &(thr->c_st);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_instruction_index_t sll_thread_get_instruction_index(sll_thread_index_t t){
	thread_data_t* thr=(t==SLL_UNKNOWN_THREAD_INDEX?_scheduler_current_thread:_thread_get(t));
	return (thr?thr->ii:0);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_thread_restart(sll_thread_index_t t){
	if (t>=_thread_len){
		return 0;
	}
	thread_data_t* thr=*(_thread_data+t);
	if (THREAD_IS_UNUSED(thr)||t==_scheduler_current_thread_index){
		return 0;
	}
	if ((thr->flags&THREAD_FLAG_SUSPENDED)&&thr->st==THREAD_STATE_QUEUED){
		_scheduler_queue_thread(t);
	}
	thr->flags&=~THREAD_FLAG_SUSPENDED;
	return 1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_thread_start(sll_thread_index_t t){
	if (t>=_thread_len){
		return 0;
	}
	thread_data_t* thr=*(_thread_data+t);
	if (THREAD_IS_UNUSED(thr)||thr->st!=THREAD_STATE_INITIALIZED){
		return 0;
	}
	thr->st=THREAD_STATE_QUEUED;
	_scheduler_queue_thread(t);
	return 1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_thread_suspend(sll_thread_index_t t){
	if (t>=_thread_len){
		return 0;
	}
	thread_data_t* thr=*(_thread_data+t);
	if (THREAD_IS_UNUSED(thr)||t==_scheduler_current_thread_index){
		return 0;
	}
	thr->flags|=THREAD_FLAG_SUSPENDED;
	return 1;
}

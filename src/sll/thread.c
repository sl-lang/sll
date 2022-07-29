#include <sll/_internal/api/atexit.h>
#include <sll/_internal/sandbox.h>
#include <sll/_internal/scheduler.h>
#include <sll/_internal/vm.h>
#include <sll/audit.h>
#include <sll/common.h>
#include <sll/container.h>
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
static void* _scheduler_allocator_cache_pool[THREAD_ALLOCATOR_CACHE_POOL_SIZE];
static sll_array_length_t _scheduler_allocator_cache_pool_len;



volatile thread_list_length_t _thread_active_count;
sll_handle_container_t _thread_data;



void _thread_deinit(void){
	SLL_HANDLE_CONTAINER_ITER_CLEAR(&_thread_data,thread_data_t,thr,{
		if (thr->state==THREAD_STATE_TERMINATED){
			SLL_RELEASE(thr->return_value);
		}
		sll_gc_remove_roots(thr->stack);
		sll_gc_remove_roots(thr->tls);
		sll_gc_remove_roots(&(thr->return_value));
		SLL_CRITICAL_ERROR(sll_platform_free_page(thr,THREAD_SIZE));
	});
	while (_scheduler_allocator_cache_pool_len){
		_scheduler_allocator_cache_pool_len--;
		SLL_CRITICAL_ERROR(sll_platform_free_page(_scheduler_allocator_cache_pool[_scheduler_allocator_cache_pool_len],THREAD_SIZE));
	}
	SLL_CRITICAL(sll_platform_lock_delete(_thread_lock));
}



thread_data_t* _thread_get(sll_thread_index_t thread_index){
	return SLL_HANDLE_CONTAINER_GET(&_thread_data,thread_index);
}



void _thread_init(void){
	_thread_lock=sll_platform_lock_create(NULL);
	SLL_HANDLE_CONTAINER_INIT(&_thread_data);
	_thread_active_count=0;
	_scheduler_allocator_cache_pool_len=0;
}



sll_thread_index_t _thread_new(void){
	SLL_CRITICAL_ERROR(sll_platform_lock_acquire(_thread_lock));
	sll_size_t out;
	SLL_HANDLE_CONTAINER_ALLOC(&_thread_data,&out);
	void* ptr=NULL;
	if (_scheduler_allocator_cache_pool_len){
		_scheduler_allocator_cache_pool_len--;
		ptr=_scheduler_allocator_cache_pool[_scheduler_allocator_cache_pool_len];
	}
	else{
		ptr=sll_platform_allocate_page(THREAD_SIZE,0,NULL);
	}
	thread_data_t* n=ptr;
	n->stack=PTR(ADDR(ptr)+sizeof(thread_data_t)+sll_current_vm_config->call_stack_size*sizeof(sll_call_stack_frame_t)+sll_current_runtime_data->assembly_data->tls_variable_count*sizeof(sll_object_t));
	n->stack[0]=NULL;
	sll_gc_add_roots(n->stack,sll_current_vm_config->stack_size);
	n->tls=PTR(ADDR(ptr)+sizeof(thread_data_t)+sll_current_vm_config->call_stack_size*sizeof(sll_call_stack_frame_t));
	sll_static_int[0]->reference_count+=sll_current_runtime_data->assembly_data->tls_variable_count;
	for (sll_variable_index_t i=0;i<sll_current_runtime_data->assembly_data->tls_variable_count;i++){
		*(n->tls+i)=sll_static_int[0];
	}
	sll_gc_add_roots(n->tls,sll_current_runtime_data->assembly_data->tls_variable_count);
	n->instruction_index=0;
	n->stack_index=0;
	n->wait=SLL_UNKNOWN_THREAD_INDEX;
	n->return_value=NULL;
	sll_gc_add_roots(&(n->return_value),1);
	n->call_stack.data=PTR(ADDR(ptr)+sizeof(thread_data_t));
	n->call_stack.length=0;
	n->sandbox=(_scheduler_current_thread_index==SLL_UNKNOWN_THREAD_INDEX?_sandbox_flags:_scheduler_current_thread->sandbox);
	n->time_quantum=THREAD_SCHEDULER_INSTRUCTION_COUNT;
	n->state=THREAD_STATE_INITIALIZED;
	n->flags=0;
	n->_last_instruction=NULL;
	*(_thread_data.data+out)=n;
	_thread_active_count++;
	SLL_CRITICAL(sll_platform_lock_release(_thread_lock));
	return (sll_thread_index_t)out;
}



void _thread_terminate(sll_object_t return_value){
	SLL_ASSERT(_scheduler_current_thread->state==THREAD_STATE_RUNNING&&!(_scheduler_current_thread->flags&THREAD_FLAG_SUSPENDED));
	SLL_ACQUIRE(return_value);
	sll_thread_index_t thread_index=_scheduler_current_thread_index;
	sll_bool_t delete=!!(_scheduler_current_thread->flags&THREAD_FLAG_DELETE);
	_scheduler_current_thread->return_value=return_value;
	_scheduler_current_thread->state=THREAD_STATE_TERMINATED;
	_scheduler_current_thread_index=_scheduler_current_thread->wait;
	if (_scheduler_current_thread_index!=SLL_UNKNOWN_THREAD_INDEX){
		_scheduler_current_thread=*(_thread_data.data+_scheduler_current_thread_index);
		while (_scheduler_current_thread->next!=SLL_UNKNOWN_THREAD_INDEX){
			_scheduler_current_thread->state=THREAD_STATE_QUEUED;
			if (_scheduler_current_thread->flags&THREAD_FLAG_SUSPENDED){
				SLL_UNIMPLEMENTED();
			}
			_scheduler_queue_thread(_scheduler_current_thread_index);
			_scheduler_current_thread_index=_scheduler_current_thread->next;
			_scheduler_current_thread=*(_thread_data.data+_scheduler_current_thread_index);
		}
		_scheduler_current_thread->state=THREAD_STATE_RUNNING;
	}
	if (!(_scheduler_current_thread->flags&THREAD_FLAG_NO_AUDIT_TERMINATE)){
		sll_audit(SLL_CHAR("sll.thread.return"),SLL_CHAR("u"),thread_index);
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
	if (delete){
		SLL_CRITICAL(sll_thread_delete(thread_index));
	}
}



sll_bool_t _thread_wait(sll_integer_t thread_index){
	if (thread_index<0||thread_index==_scheduler_current_thread_index||!SLL_HANDLE_CONTAINER_CHECK(&_thread_data,(sll_size_t)thread_index)){
		return 0;
	}
	thread_data_t* thr=*(_thread_data.data+thread_index);
	if (thr->return_value){
		return 0;
	}
	_scheduler_current_thread->time_quantum=THREAD_SCHEDULER_INSTRUCTION_COUNT;
	_scheduler_current_thread->next=thr->wait;
	_scheduler_current_thread->state=THREAD_STATE_WAIT_THREAD;
	thr->wait=_scheduler_current_thread_index;
	_scheduler_current_thread_index=SLL_UNKNOWN_THREAD_INDEX;
	return 1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_thread_index_t sll_thread_create(sll_integer_t function,const sll_object_t* args,sll_arg_count_t arg_count){
	if (function&&function<=sll_current_runtime_data->assembly_data->function_table.length){
		sll_thread_index_t out=_thread_new();
		thread_data_t* thr=*(_thread_data.data+out);
		sll_copy_objects(args,arg_count,thr->stack);
		thr->stack_index+=arg_count;
		_call_function(thr,(sll_function_index_t)(function-1),arg_count,0);
		return out;
	}
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_thread_delete(sll_thread_index_t thread_index){
	if (!thread_index||!SLL_HANDLE_CONTAINER_CHECK(&_thread_data,thread_index)){
		return 0;
	}
	thread_data_t* thr=*(_thread_data.data+thread_index);
	SLL_CRITICAL_ERROR(sll_platform_lock_acquire(_thread_lock));
	if (thr->state!=THREAD_STATE_TERMINATED){
		thr->flags|=THREAD_FLAG_DELETE;
		SLL_CRITICAL(sll_platform_lock_release(_thread_lock));
		return 1;
	}
	SLL_HANDLE_CONTAINER_DEALLOC(&_thread_data,thread_index);
	SLL_RELEASE(thr->return_value);
	for (sll_variable_index_t i=0;i<sll_current_runtime_data->assembly_data->tls_variable_count;i++){
		SLL_RELEASE(*(thr->tls+i));
	}
	SLL_ASSERT(!thr->stack_index);
	sll_gc_remove_roots(thr->stack);
	sll_gc_remove_roots(thr->tls);
	sll_gc_remove_roots(&(thr->return_value));
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



__SLL_EXTERNAL __SLL_CHECK_OUTPUT const sll_call_stack_t* sll_thread_get_call_stack(sll_thread_index_t thread_index){
	if (!SLL_HANDLE_CONTAINER_CHECK(&_thread_data,thread_index)){
		return NULL;
	}
	thread_data_t* thr=*(_thread_data.data+thread_index);
	return &(thr->call_stack);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_instruction_index_t sll_thread_get_instruction_index(sll_thread_index_t thread_index){
	thread_data_t* thr=(thread_index==SLL_UNKNOWN_THREAD_INDEX?_scheduler_current_thread:_thread_get(thread_index));
	return (thr?thr->instruction_index:0);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_thread_restart(sll_thread_index_t thread_index){
	if (thread_index==_scheduler_current_thread_index||!SLL_HANDLE_CONTAINER_CHECK(&_thread_data,thread_index)){
		return 0;
	}
	thread_data_t* thr=*(_thread_data.data+thread_index);
	if ((thr->flags&THREAD_FLAG_SUSPENDED)&&thr->state==THREAD_STATE_QUEUED){
		_scheduler_queue_thread(thread_index);
	}
	thr->flags&=~THREAD_FLAG_SUSPENDED;
	return 1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_thread_start(sll_thread_index_t thread_index){
	if (!SLL_HANDLE_CONTAINER_CHECK(&_thread_data,thread_index)){
		return 0;
	}
	thread_data_t* thr=*(_thread_data.data+thread_index);
	if (thr->state!=THREAD_STATE_INITIALIZED){
		return 0;
	}
	thr->state=THREAD_STATE_QUEUED;
	_scheduler_queue_thread(thread_index);
	return 1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_thread_suspend(sll_thread_index_t thread_index){
	if (thread_index==_scheduler_current_thread_index||!SLL_HANDLE_CONTAINER_CHECK(&_thread_data,thread_index)){
		return 0;
	}
	thread_data_t* thr=*(_thread_data.data+thread_index);
	thr->flags|=THREAD_FLAG_SUSPENDED;
	return 1;
}

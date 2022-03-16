#include <sll/_internal/barrier.h>
#include <sll/_internal/common.h>
#include <sll/_internal/dispatcher.h>
#include <sll/_internal/lock.h>
#include <sll/_internal/scheduler.h>
#include <sll/_internal/semaphore.h>
#include <sll/_internal/thread.h>
#include <sll/operator.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/object.h>
#include <sll/platform/event.h>
#include <sll/platform/memory.h>
#include <sll/platform/thread.h>
#include <sll/platform/util.h>
#include <sll/scheduler.h>
#include <sll/static_object.h>
#include <sll/thread.h>
#include <sll/types.h>
#include <sll/vm.h>



static scheduler_cpu_data_t* _scheduler_root_cpu;
static __SLL_TLS scheduler_cpu_data_t* _scheduler_data;



__SLL_TLS sll_thread_index_t _scheduler_current_thread_index;
__SLL_TLS thread_data_t* _scheduler_current_thread;



static void _cpu_core_worker(void* dt){
	_scheduler_data=dt;
	if (!sll_platform_set_cpu(_scheduler_data->id)){
		SLL_UNIMPLEMENTED();
	}
	if (_scheduler_data->id){
		return;
	}
	_scheduler_current_thread_index=SLL_UNKNOWN_THREAD_INDEX;
	_scheduler_current_thread=NULL;
	while (_thread_active_count){
		sll_thread_index_t n_tid=_scheduler_queue_pop();
		if (n_tid!=SLL_UNKNOWN_THREAD_INDEX){
			sll_release_object(sll_wait_thread(n_tid));
		}
	}
	_scheduler_current_thread_index=SLL_UNKNOWN_THREAD_INDEX;
	_scheduler_current_thread=NULL;
}



void _scheduler_queue_next(void){
	_scheduler_current_thread->tm=THREAD_SCHEDULER_INSTRUCTION_COUNT;
	if (!_scheduler_data->queue_len){
		return;
	}
	_scheduler_current_thread->st=THREAD_STATE_QUEUED;
	sll_thread_index_t tmp=*(_scheduler_data->queue+_scheduler_data->queue_idx);
	*(_scheduler_data->queue+_scheduler_data->queue_idx)=_scheduler_current_thread_index;
	if (!_scheduler_data->queue_idx){
		_scheduler_data->queue_idx=_scheduler_data->queue_len;
	}
	_scheduler_data->queue_idx--;
	_scheduler_current_thread_index=tmp;
	_scheduler_current_thread=*(_thread_data+tmp);
	if (_scheduler_current_thread->suspended){
		_scheduler_current_thread_index=_scheduler_queue_pop();
		_scheduler_current_thread=*(_thread_data+_scheduler_current_thread_index);
	}
	_scheduler_current_thread->st=THREAD_STATE_RUNNING;
}



sll_thread_index_t _scheduler_queue_pop(void){
	if (!_scheduler_data->queue_len){
		if (!_thread_active_count){
			return SLL_UNKNOWN_THREAD_INDEX;
		}
		_scheduler_data->wait=1;
		if (!sll_platform_event_wait(_scheduler_data->sig)){
			SLL_UNIMPLEMENTED();
		}
		_scheduler_data->wait=0;
		if (!_scheduler_data->queue_len){
			return SLL_UNKNOWN_THREAD_INDEX;
		}
	}
	sll_thread_index_t o=*(_scheduler_data->queue+_scheduler_data->queue_idx);
	if ((*(_thread_data+o))->suspended){
		SLL_UNIMPLEMENTED();
	}
	for (queue_length_t i=_scheduler_data->queue_idx+1;i<_scheduler_data->queue_len;i++){
		*(_scheduler_data->queue+i-1)=*(_scheduler_data->queue+i);
	}
	_scheduler_data->queue_len--;
	if (!_scheduler_data->queue_len){
		_scheduler_data->queue_idx=0;
	}
	else{
		if (!_scheduler_data->queue_idx){
			_scheduler_data->queue_idx=_scheduler_data->queue_len;
		}
		_scheduler_data->queue_idx--;
	}
	(*(_thread_data+o))->st=THREAD_STATE_UNDEFINED;
	return o;
}



void _scheduler_queue_thread(sll_bool_t any,sll_thread_index_t t){
	scheduler_cpu_data_t* cpu_dt=(any?_scheduler_root_cpu:_scheduler_data);
	cpu_dt->queue_len++;
	*(cpu_dt->queue+cpu_dt->queue_len-1)=t;
	if (cpu_dt->wait&&cpu_dt->queue_len==1){
		if (!sll_platform_event_set(cpu_dt->sig)){
			SLL_UNIMPLEMENTED();
		}
	}
}



sll_return_code_t _scheduler_run(void){
	_thread_init();
	_io_dispatcher_init();
	_barrier_init();
	_lock_init();
	_semaphore_init();
	_scheduler_current_thread_index=SLL_UNKNOWN_THREAD_INDEX;
	_scheduler_current_thread=NULL;
	sll_size_t sz=(sizeof(sll_thread_index_t)+SLL_SCHEDULER_MAX_THREADS/(*sll_platform_cpu_count)*sizeof(scheduler_cpu_data_t)+7)&0xfffffffffffffff8ull;
	void* b_ptr=sll_platform_allocate_page(SLL_ROUND_PAGE((*sll_platform_cpu_count)*sz),0);
	_scheduler_root_cpu=b_ptr;
	void* ptr=b_ptr;
	for (sll_cpu_t i=0;i<*sll_platform_cpu_count;i++){
		scheduler_cpu_data_t* cpu_dt=ptr;
		cpu_dt->queue_idx=0;
		cpu_dt->queue_len=!i;
		cpu_dt->sig=sll_platform_event_create();
		cpu_dt->id=i;
		cpu_dt->wait=0;
		if (!i){
			cpu_dt->tid=sll_platform_current_thread();
			*(cpu_dt->queue)=_thread_new();
		}
		else{
			cpu_dt->tid=sll_platform_start_thread(_cpu_core_worker,cpu_dt);
		}
		ptr=PTR(ADDR(ptr)+sz);
	}
	_cpu_core_worker(b_ptr);
	if (!sll_platform_set_cpu(SLL_CPU_ANY)){
		SLL_UNIMPLEMENTED();
	}
	ptr=b_ptr;
	for (sll_cpu_t i=0;i<*sll_platform_cpu_count;i++){
		scheduler_cpu_data_t* cpu_dt=ptr;
		if (!sll_platform_event_set(cpu_dt->sig)){
			SLL_UNIMPLEMENTED();
		}
		if (i&&!sll_platform_join_thread(cpu_dt->tid)){
			SLL_UNIMPLEMENTED();
		}
		if (!sll_platform_event_delete(cpu_dt->sig)){
			SLL_UNIMPLEMENTED();
		}
		ptr=PTR(ADDR(ptr)+sz);
	}
	sll_platform_free_page(b_ptr,SLL_ROUND_PAGE((*sll_platform_cpu_count)*sz));
	sll_object_t* rc_o=sll_operator_cast(_thread_get(0)->ret,sll_static_int[SLL_OBJECT_TYPE_INT]);
	sll_return_code_t o=(sll_return_code_t)(rc_o->dt.i);
	sll_release_object(rc_o);
	_barrier_deinit();
	_lock_deinit();
	_semaphore_deinit();
	_io_dispatcher_deinit();
	_thread_deinit();
	return o;
}



void _scheduler_set_thread(sll_thread_index_t t){
	if (_scheduler_current_thread_index==t){
		return;
	}
	if (_scheduler_current_thread_index!=SLL_UNKNOWN_THREAD_INDEX){
		_scheduler_queue_thread(0,_scheduler_current_thread_index);
	}
	if ((*(_thread_data+t))->st==THREAD_STATE_TERMINATED){
		_scheduler_current_thread_index=SLL_UNKNOWN_THREAD_INDEX;
		return;
	}
	_scheduler_current_thread_index=t;
	_scheduler_current_thread=*(_thread_data+t);
	if ((_scheduler_current_thread->st!=THREAD_STATE_INITIALIZED&&_scheduler_current_thread->st!=THREAD_STATE_RUNNING&&_scheduler_current_thread->st!=THREAD_STATE_UNDEFINED&&_scheduler_current_thread->st!=THREAD_STATE_WAIT_IO)||_scheduler_current_thread->suspended){
		SLL_UNIMPLEMENTED();
	}
	_scheduler_current_thread->st=THREAD_STATE_RUNNING;
}

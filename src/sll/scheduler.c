#include <sll/_internal/audit.h>
#include <sll/_internal/barrier.h>
#include <sll/_internal/common.h>
#include <sll/_internal/dispatcher.h>
#include <sll/_internal/init.h>
#include <sll/_internal/lock.h>
#include <sll/_internal/scheduler.h>
#include <sll/_internal/semaphore.h>
#include <sll/_internal/thread.h>
#include <sll/_internal/vm.h>
#include <sll/audit.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/object.h>
#include <sll/operator.h>
#include <sll/platform/event.h>
#include <sll/platform/lock.h>
#include <sll/platform/memory.h>
#include <sll/platform/process.h>
#include <sll/platform/thread.h>
#include <sll/platform/util.h>
#include <sll/scheduler.h>
#include <sll/static_object.h>
#include <sll/thread.h>
#include <sll/types.h>
#include <sll/vm.h>



static load_balancer_t _scheduler_load_balancer;
static scheduler_cpu_data_t* _scheduler_data_base;



__SLL_TLS sll_cpu_t _scheduler_internal_thread_index;
__SLL_TLS sll_thread_index_t _scheduler_current_thread_index=SLL_UNKNOWN_THREAD_INDEX;
__SLL_TLS thread_data_t* _scheduler_current_thread;



static void _cpu_core_worker(void* dt){
	_scheduler_internal_thread_index=(sll_cpu_t)ADDR(dt);
#ifndef __SLL_BUILD_FUZZER
	SLL_CRITICAL_ERROR(sll_platform_set_cpu(_scheduler_internal_thread_index));
#endif
	if (_scheduler_internal_thread_index){
		return;
	}
	_scheduler_current_thread_index=SLL_UNKNOWN_THREAD_INDEX;
	_scheduler_current_thread=NULL;
	while (_thread_active_count){
		sll_thread_index_t n_tid=_scheduler_queue_pop();
		if (n_tid!=SLL_UNKNOWN_THREAD_INDEX){
			SLL_RELEASE(sll_wait_thread(n_tid));
		}
	}
	_scheduler_current_thread_index=SLL_UNKNOWN_THREAD_INDEX;
	_scheduler_current_thread=NULL;
}



void _scheduler_queue_next(void){
	scheduler_cpu_data_t* cpu_data=_scheduler_data_base+_scheduler_internal_thread_index;
	_scheduler_current_thread->time_quantum=THREAD_SCHEDULER_INSTRUCTION_COUNT;
	if (!cpu_data->queue_len){
		return;
	}
	_scheduler_current_thread->state=THREAD_STATE_QUEUED;
	sll_thread_index_t tmp=*(cpu_data->queue+cpu_data->queue_idx);
	*(cpu_data->queue+cpu_data->queue_idx)=_scheduler_current_thread_index;
	if (!cpu_data->queue_idx){
		cpu_data->queue_idx=cpu_data->queue_len;
	}
	cpu_data->queue_idx--;
	_scheduler_current_thread_index=tmp;
	_scheduler_current_thread=*(_thread_data.data+tmp);
	if (_scheduler_current_thread->flags&THREAD_FLAG_SUSPENDED){
		_scheduler_current_thread_index=_scheduler_queue_pop();
		_scheduler_current_thread=*(_thread_data.data+_scheduler_current_thread_index);
	}
	_scheduler_current_thread->state=THREAD_STATE_RUNNING;
}



sll_thread_index_t _scheduler_queue_pop(void){
	scheduler_cpu_data_t* cpu_data=_scheduler_data_base+_scheduler_internal_thread_index;
	if (!cpu_data->queue_len){
		if (!_thread_active_count){
			return SLL_UNKNOWN_THREAD_INDEX;
		}
		cpu_data->wait=1;
		SLL_CRITICAL_ERROR(sll_platform_event_wait(cpu_data->signal_event));
		cpu_data->wait=0;
		if (!cpu_data->queue_len){
			return SLL_UNKNOWN_THREAD_INDEX;
		}
	}
	sll_thread_index_t out=*(cpu_data->queue+cpu_data->queue_idx);
	thread_data_t* thr=*(_thread_data.data+out);
	if (thr->flags&THREAD_FLAG_SUSPENDED){
		SLL_UNIMPLEMENTED();
	}
	for (queue_length_t i=cpu_data->queue_idx+1;i<cpu_data->queue_len;i++){
		*(cpu_data->queue+i-1)=*(cpu_data->queue+i);
	}
	cpu_data->queue_len--;
	if (!cpu_data->queue_len){
		cpu_data->queue_idx=0;
	}
	else{
		if (!cpu_data->queue_idx){
			cpu_data->queue_idx=cpu_data->queue_len;
		}
		cpu_data->queue_idx--;
	}
	thr->state=THREAD_STATE_UNDEFINED;
	return out;
}



void _scheduler_queue_thread(sll_thread_index_t t){
	SLL_CRITICAL_ERROR(sll_platform_lock_acquire(_scheduler_load_balancer.lock));
	_scheduler_load_balancer.index=0/*(!_scheduler_load_balancer.brk?_scheduler_load_balancer.len:_scheduler_load_balancer.brk)-1*/;
	scheduler_cpu_data_t* cpu_data=_scheduler_data_base+_scheduler_load_balancer.index;
	SLL_CRITICAL_ERROR(sll_platform_lock_acquire(cpu_data->lock));
	cpu_data->queue_len++;
	*(cpu_data->queue+cpu_data->queue_len-1)=t;
	thread_data_t* thr=*(_thread_data.data+t);
	thr->state=THREAD_STATE_QUEUED;
	if (cpu_data->wait&&cpu_data->queue_len==1){
		SLL_CRITICAL_ERROR(sll_platform_event_set(cpu_data->signal_event));
	}
	SLL_CRITICAL(sll_platform_lock_release(cpu_data->lock));
	SLL_CRITICAL(sll_platform_lock_release(_scheduler_load_balancer.lock));
}



sll_return_code_t _scheduler_run(void){
	_thread_init();
	_io_dispatcher_init();
	_barrier_init();
	_lock_init();
	_semaphore_init();
	_scheduler_current_thread_index=SLL_UNKNOWN_THREAD_INDEX;
	_scheduler_current_thread=NULL;
	_scheduler_load_balancer.length=*sll_platform_cpu_count;
	if (_scheduler_load_balancer.length>255){
		_scheduler_load_balancer.length=255;
	}
	_scheduler_data_base=sll_platform_allocate_page(SLL_ROUND_PAGE(_scheduler_load_balancer.length*sizeof(scheduler_cpu_data_t)),0,NULL);
	_scheduler_load_balancer.lock=sll_platform_lock_create(NULL);
	_scheduler_load_balancer.index=0;
	scheduler_cpu_data_t* cpu_data=_scheduler_data_base;
	for (sll_cpu_t i=0;i<_scheduler_load_balancer.length;i++){
		cpu_data->queue_idx=0;
		cpu_data->queue_len=!i;
		cpu_data->signal_event=sll_platform_event_create(NULL);
		cpu_data->lock=sll_platform_lock_create(NULL);
		cpu_data->cpu_id=i;
		cpu_data->wait=0;
		if (!i){
			cpu_data->internal_thread_index=sll_platform_current_thread();
			*(cpu_data->queue)=_thread_new();
			sll_audit(SLL_CHAR("sll.thread.create"),SLL_CHAR("0Ai"),*(cpu_data->queue));
		}
		else{
			cpu_data->internal_thread_index=sll_platform_start_thread(_cpu_core_worker,PTR(i),NULL);
		}
		cpu_data++;
	}
	_cpu_core_worker(NULL);
	sll_object_t rc_object=sll_operator_cast(_thread_get(0)->return_value,sll_static_int[SLL_OBJECT_TYPE_INT]);
	sll_return_code_t out=(sll_return_code_t)(rc_object->data.int_);
	SLL_RELEASE(rc_object);
	sll_gc_collect();
	_cleanup_vm_exit_tables();
	_release_var_data();
	sll_gc_collect();
	SLL_CRITICAL_ERROR(sll_platform_set_cpu(SLL_CPU_ANY));
	cpu_data=_scheduler_data_base;
	for (sll_cpu_t i=0;i<_scheduler_load_balancer.length;i++){
		if (i){
			SLL_CRITICAL_ERROR(sll_platform_event_set(cpu_data->signal_event));
			SLL_CRITICAL_ERROR_COND(i,sll_platform_join_thread(cpu_data->internal_thread_index));
		}
		SLL_CRITICAL_ERROR(sll_platform_event_delete(cpu_data->signal_event));
		SLL_CRITICAL(sll_platform_lock_delete(cpu_data->lock));
		cpu_data++;
	}
	SLL_CRITICAL(sll_platform_lock_delete(_scheduler_load_balancer.lock));
	SLL_CRITICAL_ERROR(sll_platform_free_page(_scheduler_data_base,SLL_ROUND_PAGE(_scheduler_load_balancer.length*sizeof(scheduler_cpu_data_t))));
	_barrier_deinit();
	_lock_deinit();
	_semaphore_deinit();
	_io_dispatcher_deinit();
	_thread_deinit();
	_scheduler_current_thread_index=SLL_UNKNOWN_THREAD_INDEX;
	return out;
}



void _scheduler_set_thread(sll_thread_index_t thread_index){
	if (_scheduler_current_thread_index==thread_index){
		return;
	}
	if (_scheduler_current_thread_index!=SLL_UNKNOWN_THREAD_INDEX){
		_scheduler_queue_thread(_scheduler_current_thread_index);
	}
	thread_data_t* thr=*(_thread_data.data+thread_index);
	if (thr->state==THREAD_STATE_TERMINATED){
		_scheduler_current_thread_index=SLL_UNKNOWN_THREAD_INDEX;
		return;
	}
	_scheduler_current_thread_index=thread_index;
	_scheduler_current_thread=thr;
	if ((_scheduler_current_thread->state!=THREAD_STATE_INITIALIZED&&_scheduler_current_thread->state!=THREAD_STATE_RUNNING&&_scheduler_current_thread->state!=THREAD_STATE_UNDEFINED&&_scheduler_current_thread->state!=THREAD_STATE_WAIT_IO)||(_scheduler_current_thread->flags&THREAD_FLAG_SUSPENDED)){
		SLL_UNIMPLEMENTED();
	}
	_scheduler_current_thread->state=THREAD_STATE_RUNNING;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_cpu_t sll_get_cpu_index(void){
	return _scheduler_internal_thread_index;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_thread_index_t sll_get_thread_index(void){
	return _scheduler_current_thread_index;
}

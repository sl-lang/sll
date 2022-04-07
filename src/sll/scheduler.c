#include <sll/_internal/audit.h>
#include <sll/_internal/barrier.h>
#include <sll/_internal/common.h>
#include <sll/_internal/dispatcher.h>
#include <sll/_internal/gc.h>
#include <sll/_internal/lock.h>
#include <sll/_internal/scheduler.h>
#include <sll/_internal/semaphore.h>
#include <sll/_internal/thread.h>
#include <sll/audit.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/object.h>
#include <sll/operator.h>
#include <sll/platform/event.h>
#include <sll/platform/lock.h>
#include <sll/platform/memory.h>
#include <sll/platform/thread.h>
#include <sll/platform/util.h>
#include <sll/scheduler.h>
#include <sll/static_object.h>
#include <sll/thread.h>
#include <sll/types.h>
#include <sll/vm.h>



static load_balancer_t _scheduler_load_balancer;
static __SLL_TLS scheduler_cpu_data_t* _scheduler_data;



__SLL_TLS sll_cpu_t _scheduler_internal_thread_index;
__SLL_TLS sll_thread_index_t _scheduler_current_thread_index;
__SLL_TLS thread_data_t* _scheduler_current_thread;



static void _cpu_core_worker(void* dt){
	_scheduler_data=dt;
	SLL_CRITICAL(sll_platform_set_cpu(_scheduler_data->id));
	_scheduler_internal_thread_index=_scheduler_data->id;
	if (_scheduler_data->id){
		return;
	}
	_scheduler_current_thread_index=SLL_UNKNOWN_THREAD_INDEX;
	_scheduler_current_thread=NULL;
	while (_thread_active_count){
		sll_thread_index_t n_tid=_scheduler_queue_pop(1);
		if (n_tid!=SLL_UNKNOWN_THREAD_INDEX){
			GC_RELEASE(sll_wait_thread(n_tid));
		}
	}
	_scheduler_current_thread_index=SLL_UNKNOWN_THREAD_INDEX;
	_scheduler_current_thread=NULL;
}



void _scheduler_queue_next(void){
	_scheduler_current_thread->tm=THREAD_SCHEDULER_INSTRUCTION_COUNT;
	SLL_CRITICAL(sll_platform_lock_acquire(_scheduler_data->lck));
	if (!_scheduler_data->queue_len){
		goto _end;
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
	if (_scheduler_current_thread->flags&THREAD_FLAG_SUSPENDED){
		SLL_CRITICAL(sll_platform_lock_release(_scheduler_data->lck));
		_scheduler_current_thread_index=_scheduler_queue_pop(0);
		SLL_CRITICAL(sll_platform_lock_acquire(_scheduler_data->lck));
		_scheduler_current_thread=*(_thread_data+_scheduler_current_thread_index);
	}
	_scheduler_current_thread->st=THREAD_STATE_RUNNING;
_end:
	SLL_CRITICAL(sll_platform_lock_release(_scheduler_data->lck));
}



sll_thread_index_t _scheduler_queue_pop(sll_bool_t lck){
	SLL_CRITICAL_COND(lck,sll_platform_lock_acquire(_scheduler_data->lck));
	sll_thread_index_t o=SLL_UNKNOWN_THREAD_INDEX;
	if (!_scheduler_data->queue_len){
		if (!_thread_active_count){
			goto _end;
		}
		_scheduler_data->wait=1;
		SLL_CRITICAL(sll_platform_lock_release(_scheduler_data->lck));
		SLL_CRITICAL(sll_platform_event_wait(_scheduler_data->evt));
		SLL_CRITICAL(sll_platform_lock_acquire(_scheduler_data->lck));
		_scheduler_data->wait=0;
		if (!_scheduler_data->queue_len){
			goto _end;
		}
	}
	o=*(_scheduler_data->queue+_scheduler_data->queue_idx);
	if ((*(_thread_data+o))->flags&THREAD_FLAG_SUSPENDED){
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
_end:
	SLL_CRITICAL_COND(lck,sll_platform_lock_release(_scheduler_data->lck));
	return o;
}



void _scheduler_queue_thread(sll_thread_index_t t){
	SLL_CRITICAL(sll_platform_lock_acquire(_scheduler_load_balancer.lck));
	_scheduler_load_balancer.brk=0/*(!_scheduler_load_balancer.brk?_scheduler_load_balancer.len:_scheduler_load_balancer.brk)-1*/;
	scheduler_cpu_data_t* cpu_dt=_scheduler_load_balancer.dt[_scheduler_load_balancer.brk];
	SLL_CRITICAL(sll_platform_lock_acquire(cpu_dt->lck));
	cpu_dt->queue_len++;
	*(cpu_dt->queue+cpu_dt->queue_len-1)=t;
	(*(_thread_data+t))->st=THREAD_STATE_QUEUED;
	if (cpu_dt->wait&&cpu_dt->queue_len==1){
		SLL_CRITICAL(sll_platform_event_set(cpu_dt->evt));
	}
	SLL_CRITICAL(sll_platform_lock_release(cpu_dt->lck));
	SLL_CRITICAL(sll_platform_lock_release(_scheduler_load_balancer.lck));
}



sll_return_code_t _scheduler_run(void){
	_thread_init();
	_io_dispatcher_init();
	_barrier_init();
	_lock_init();
	_semaphore_init();
	_scheduler_current_thread_index=SLL_UNKNOWN_THREAD_INDEX;
	_scheduler_current_thread=NULL;
	_scheduler_load_balancer.len=*sll_platform_cpu_count;
	if (_scheduler_load_balancer.len>255){
		_scheduler_load_balancer.len=255;
	}
	sll_size_t sz=(sizeof(sll_thread_index_t)+SLL_SCHEDULER_MAX_THREADS/_scheduler_load_balancer.len*sizeof(scheduler_cpu_data_t)+7)&0xfffffffffffffff8ull;
	void* b_ptr=sll_platform_allocate_page(SLL_ROUND_PAGE(_scheduler_load_balancer.len*(sz+sizeof(scheduler_cpu_data_t*))),0);
	_scheduler_load_balancer.dt=PTR(ADDR(b_ptr)+_scheduler_load_balancer.len*sz);
	_scheduler_load_balancer.lck=sll_platform_lock_create();
	_scheduler_load_balancer.brk=0;
	void* ptr=b_ptr;
	for (sll_cpu_t i=0;i<_scheduler_load_balancer.len;i++){
		scheduler_cpu_data_t* cpu_dt=ptr;
		*(_scheduler_load_balancer.dt+i)=cpu_dt;
		cpu_dt->queue_idx=0;
		cpu_dt->queue_len=!i;
		cpu_dt->evt=sll_platform_event_create();
		cpu_dt->lck=sll_platform_lock_create();
		cpu_dt->id=i;
		cpu_dt->wait=0;
		if (!i){
			cpu_dt->tid=sll_platform_current_thread();
			*(cpu_dt->queue)=_thread_new();
			sll_audit(SLL_CHAR("sll.thread.create"),SLL_CHAR("0Ai"),*(cpu_dt->queue));
		}
		else{
			cpu_dt->tid=sll_platform_start_thread(_cpu_core_worker,cpu_dt);
		}
		ptr=PTR(ADDR(ptr)+sz);
	}
	_cpu_core_worker(b_ptr);
	_audit_cleanup_api();
	SLL_CRITICAL(sll_platform_set_cpu(SLL_CPU_ANY));
	ptr=b_ptr;
	for (sll_cpu_t i=0;i<_scheduler_load_balancer.len;i++){
		scheduler_cpu_data_t* cpu_dt=ptr;
		if (i){
			SLL_CRITICAL(sll_platform_event_set(cpu_dt->evt));
			SLL_CRITICAL_COND(i,sll_platform_join_thread(cpu_dt->tid));
		}
		SLL_CRITICAL(sll_platform_event_delete(cpu_dt->evt));
		SLL_CRITICAL(sll_platform_lock_delete(cpu_dt->lck));
		ptr=PTR(ADDR(ptr)+sz);
	}
	SLL_CRITICAL(sll_platform_lock_delete(_scheduler_load_balancer.lck));
	sll_platform_free_page(b_ptr,SLL_ROUND_PAGE(_scheduler_load_balancer.len*(sz+sizeof(scheduler_cpu_data_t*))));
	sll_object_t* rc_o=sll_operator_cast(_thread_get(0)->ret,sll_static_int[SLL_OBJECT_TYPE_INT]);
	sll_return_code_t o=(sll_return_code_t)(rc_o->dt.i);
	GC_RELEASE(rc_o);
	_barrier_deinit();
	_lock_deinit();
	_semaphore_deinit();
	_io_dispatcher_deinit();
	_thread_deinit();
	return o;
}



void _scheduler_set_thread(sll_thread_index_t t){
	SLL_CRITICAL(sll_platform_lock_acquire(_scheduler_data->lck));
	if (_scheduler_current_thread_index==t){
		goto _end;
	}
	if (_scheduler_current_thread_index!=SLL_UNKNOWN_THREAD_INDEX){
		SLL_CRITICAL(sll_platform_lock_release(_scheduler_data->lck));
		_scheduler_queue_thread(_scheduler_current_thread_index);
		SLL_CRITICAL(sll_platform_lock_acquire(_scheduler_data->lck));
	}
	if ((*(_thread_data+t))->st==THREAD_STATE_TERMINATED){
		_scheduler_current_thread_index=SLL_UNKNOWN_THREAD_INDEX;
		goto _end;
	}
	_scheduler_current_thread_index=t;
	_scheduler_current_thread=*(_thread_data+t);
	if ((_scheduler_current_thread->st!=THREAD_STATE_INITIALIZED&&_scheduler_current_thread->st!=THREAD_STATE_RUNNING&&_scheduler_current_thread->st!=THREAD_STATE_UNDEFINED&&_scheduler_current_thread->st!=THREAD_STATE_WAIT_IO)||(_scheduler_current_thread->flags&THREAD_FLAG_SUSPENDED)){
		SLL_UNIMPLEMENTED();
	}
	_scheduler_current_thread->st=THREAD_STATE_RUNNING;
_end:
	SLL_CRITICAL(sll_platform_lock_release(_scheduler_data->lck));
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_cpu_t sll_get_cpu_index(void){
	return _scheduler_internal_thread_index;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_thread_index_t sll_get_thread_index(void){
	return _scheduler_current_thread_index;
}

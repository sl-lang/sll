#include <sll/_internal/common.h>
#include <sll/_internal/dispatcher.h>
#include <sll/_internal/scheduler.h>
#include <sll/_internal/thread.h>
#include <sll/common.h>
#include <sll/memory.h>
#include <sll/thread.h>
#include <sll/types.h>



static sll_thread_index_t* _scheduler_queue;
static queue_length_t _scheduler_queue_idx;
static queue_length_t _scheduler_queue_len;



thread_data_t* _scheduler_current_thread;



__SLL_EXTERNAL sll_thread_index_t sll_current_thread_index=0;



void _scheduler_deinit(void){
	sll_deallocate(_scheduler_queue);
	_scheduler_current_thread=NULL;
	sll_current_thread_index=SLL_UNKNOWN_THREAD_INDEX;
}



void _scheduler_init(void){
	_scheduler_queue=NULL;
	_scheduler_queue_idx=0;
	_scheduler_queue_len=0;
	_scheduler_current_thread=NULL;
	sll_current_thread_index=SLL_UNKNOWN_THREAD_INDEX;
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
	_scheduler_current_thread=*(_thread_data+tmp);
	if (_scheduler_current_thread->suspended){
		sll_current_thread_index=_scheduler_queue_pop();
		_scheduler_current_thread=*(_thread_data+sll_current_thread_index);
	}
	_scheduler_current_thread->st=THREAD_STATE_RUNNING;
}



sll_thread_index_t _scheduler_queue_pop(void){
	if (!_scheduler_queue_len){
		sll_thread_index_t tid=_io_dispatcher_wait();
		if (tid==SLL_UNKNOWN_THREAD_INDEX){
			return SLL_UNKNOWN_THREAD_INDEX;
		}
		(*(_thread_data+tid))->st=THREAD_STATE_UNDEFINED;
		return tid;
	}
	sll_thread_index_t o=*(_scheduler_queue+_scheduler_queue_idx);
	if ((*(_thread_data+o))->suspended){
		SLL_UNIMPLEMENTED();
	}
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
	(*(_thread_data+o))->st=THREAD_STATE_UNDEFINED;
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
	}
	if ((*(_thread_data+t))->st==THREAD_STATE_TERMINATED){
		sll_current_thread_index=SLL_UNKNOWN_THREAD_INDEX;
		return;
	}
	sll_current_thread_index=t;
	_scheduler_current_thread=*(_thread_data+t);
	if ((_scheduler_current_thread->st!=THREAD_STATE_INITIALIZED&&_scheduler_current_thread->st!=THREAD_STATE_RUNNING&&_scheduler_current_thread->st!=THREAD_STATE_UNDEFINED&&_scheduler_current_thread->st!=THREAD_STATE_WAIT_IO)||_scheduler_current_thread->suspended){
		SLL_UNIMPLEMENTED();
	}
	_scheduler_current_thread->st=THREAD_STATE_RUNNING;
}

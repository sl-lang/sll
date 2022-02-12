#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/memory.h>
#include <sll/platform.h>
#include <sll/types.h>
#include <sll/vm.h>



static sll_thread_index_t* _scheduler_wait_list;
static wait_list_length_t _scheduler_wait_list_len;
static sll_thread_index_t* _scheduler_queue;
static queue_length_t _scheduler_queue_idx;
static queue_length_t _scheduler_queue_len;



thread_data_t** _vm_thr;
thread_data_t* _vm_current_thread;
sll_thread_index_t _vm_thr_idx;
sll_thread_index_t _vm_thr_count;



static void _add_wait_list(void){
	for (wait_list_length_t i=0;i<_scheduler_wait_list_len;i++){
		if (*(_scheduler_wait_list+i)==THREAD_UNKNOWN_INDEX){
			*(_scheduler_wait_list+i)=_vm_thr_idx;
			return;
		}
	}
	_scheduler_wait_list_len++;
	_scheduler_wait_list=sll_reallocate(_scheduler_wait_list,_scheduler_wait_list_len*sizeof(sll_thread_index_t));
	*(_scheduler_wait_list+_scheduler_wait_list_len-1)=_vm_thr_idx;
}



void _scheduler_deinit(void){
	while (_vm_thr_count){
		_scheduler_delete_thread(_vm_thr_count-1);
		_vm_thr_count--;
	}
	sll_deallocate(_vm_thr);
	sll_deallocate(_scheduler_wait_list);
}



void _scheduler_delete_thread(sll_thread_index_t t){
	if (t>=_vm_thr_count){
		return;
	}
	thread_data_t* thr=*(_vm_thr+t);
	if (!thr){
		return;
	}
	SLL_ASSERT(thr->ret);
	*(_vm_thr+t)=NULL;
	SLL_RELEASE(thr->ret);
	sll_platform_free_page(thr,SLL_ROUND_PAGE(sizeof(thread_data_t)+sll_current_vm_config->c_st_sz*sizeof(sll_call_stack_frame_t)+sll_current_vm_config->s_sz));

}



void _scheduler_init(void){
	_vm_thr=NULL;
	_vm_thr_count=0;
	_vm_thr_idx=THREAD_UNKNOWN_INDEX;
	_scheduler_wait_list=NULL;
	_scheduler_wait_list_len=0;
	_scheduler_queue=NULL;
	_scheduler_queue_idx=0;
	_scheduler_queue_len=0;
}



sll_thread_index_t _scheduler_new_thread(void){
	sll_thread_index_t o=0;
	while (o<_vm_thr_count){
		if (!*(_vm_thr+o)){
			break;
		}
		o++;
	}
	if (o==_vm_thr_count){
		_vm_thr_count++;
		_vm_thr=sll_reallocate(_vm_thr,_vm_thr_count*sizeof(thread_data_t*));
	}
	void* ptr=sll_platform_allocate_page(SLL_ROUND_PAGE(sizeof(thread_data_t)+sll_current_vm_config->c_st_sz*sizeof(sll_call_stack_frame_t)+sll_current_vm_config->s_sz),0);
	thread_data_t* n=ptr;
	n->stack=PTR(ADDR(ptr)+sizeof(thread_data_t)+sll_current_vm_config->c_st_sz*sizeof(sll_call_stack_frame_t));
	n->ii=0;
	n->si=0;
	n->wh=0;
	n->ret=NULL;
	n->c_st.dt=PTR(ADDR(ptr)+sizeof(thread_data_t));
	n->c_st.l=0;
	n->tm=THREAD_SCHEDULER_INSTRUCTION_COUNT;
	*(_vm_thr+o)=n;
	if (_vm_thr_idx==THREAD_UNKNOWN_INDEX){
		_vm_current_thread=n;
		_vm_thr_idx=o;
	}
	return o;
}



void _scheduler_queue_next(void){
	_vm_current_thread->tm=THREAD_SCHEDULER_INSTRUCTION_COUNT;
	if (!_scheduler_queue_len){
		return;
	}
	sll_thread_index_t tmp=*(_scheduler_queue+_scheduler_queue_idx);
	*(_scheduler_queue+_scheduler_queue_idx)=_vm_thr_idx;
	if (!_scheduler_queue_idx){
		_scheduler_queue_idx=_scheduler_queue_len;
	}
	_scheduler_queue_idx--;
	_vm_thr_idx=tmp;
	_vm_current_thread=*(_vm_thr+tmp);
}



sll_thread_index_t _scheduler_queue_pop(void){
	if (!_scheduler_queue_len){
		return THREAD_UNKNOWN_INDEX;
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
	return o;
}



void _scheduler_queue_thread(sll_thread_index_t t){
	_scheduler_queue_len++;
	_scheduler_queue=sll_reallocate(_scheduler_queue,_scheduler_queue_len*sizeof(sll_thread_index_t));
	*(_scheduler_queue+_scheduler_queue_len-1)=t;
}



void _scheduler_set_thread(sll_thread_index_t t){
	if (_vm_thr_idx==t){
		return;
	}
	if (_vm_thr_idx!=THREAD_UNKNOWN_INDEX){
		_scheduler_queue_thread(_vm_thr_idx);
	}
	_vm_thr_idx=t;
	_vm_current_thread=*(_vm_thr+t);
}



void _scheduler_terminate_thread(sll_object_t* ret){
	SLL_ASSERT(!_vm_current_thread->ret);
	SLL_ACQUIRE(ret);
	_vm_current_thread->ret=ret;
	wait_handle_t wh=THREAD_WAIT_HANDLE_THREAD(_vm_thr_idx);
	_vm_thr_idx=THREAD_UNKNOWN_INDEX;
	for (wait_list_length_t i=0;i<_scheduler_wait_list_len;i++){
		sll_thread_index_t j=*(_scheduler_wait_list+i);
		if (j==THREAD_UNKNOWN_INDEX){
			continue;
		}
		thread_data_t* k=*(_vm_thr+j);
		if (k->wh==wh){
			if (_vm_thr_idx==THREAD_UNKNOWN_INDEX){
				_vm_thr_idx=j;
				_vm_current_thread=k;
			}
			else{
				_scheduler_queue_thread(j);
			}
			*(_scheduler_wait_list+i)=THREAD_UNKNOWN_INDEX;
		}
	}
	if (_vm_thr_idx==THREAD_UNKNOWN_INDEX){
		_vm_thr_idx=_scheduler_queue_pop();
		if (_vm_thr_idx!=THREAD_UNKNOWN_INDEX){
			_vm_current_thread=*(_vm_thr+_vm_thr_idx);
		}
	}
}



void _scheduler_wait_thread(sll_thread_index_t w){
	if (w==_vm_thr_idx){
		SLL_UNIMPLEMENTED();
	}
	_vm_current_thread->wh=THREAD_WAIT_HANDLE_THREAD(w);
	_add_wait_list();
	_vm_thr_idx=THREAD_UNKNOWN_INDEX;
}

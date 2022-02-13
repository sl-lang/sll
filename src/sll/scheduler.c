#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/memory.h>
#include <sll/platform.h>
#include <sll/types.h>
#include <sll/vm.h>



static thread_data_t** _scheduler_thread;
static sll_thread_index_t _scheduler_thread_len;
static sll_thread_index_t* _scheduler_wait_list;
static wait_list_length_t _scheduler_wait_list_len;
static sll_thread_index_t* _scheduler_queue;
static queue_length_t _scheduler_queue_idx;
static queue_length_t _scheduler_queue_len;
static sll_thread_index_t* _scheduler_lock_list;
static lock_list_length_t _scheduler_lock_list_len;



thread_data_t* _scheduler_current_thread;
sll_thread_index_t _scheduler_thread_idx;



static void _add_wait_list(void){
	for (wait_list_length_t i=0;i<_scheduler_wait_list_len;i++){
		if (*(_scheduler_wait_list+i)==THREAD_UNKNOWN_INDEX){
			*(_scheduler_wait_list+i)=_scheduler_thread_idx;
			return;
		}
	}
	_scheduler_wait_list_len++;
	_scheduler_wait_list=sll_reallocate(_scheduler_wait_list,_scheduler_wait_list_len*sizeof(sll_thread_index_t));
	*(_scheduler_wait_list+_scheduler_wait_list_len-1)=_scheduler_thread_idx;
}



void _scheduler_deinit(void){
	while (_scheduler_thread_len){
		_scheduler_delete_thread(_scheduler_thread_len-1);
		_scheduler_thread_len--;
	}
	sll_deallocate(_scheduler_thread);
	sll_deallocate(_scheduler_wait_list);
	sll_deallocate(_scheduler_queue);
	sll_deallocate(_scheduler_lock_list);
}



void _scheduler_delete_thread(sll_thread_index_t t){
	if (t>=_scheduler_thread_len){
		return;
	}
	thread_data_t* thr=*(_scheduler_thread+t);
	if (!thr){
		return;
	}
	SLL_ASSERT(thr->ret);
	*(_scheduler_thread+t)=NULL;
	SLL_RELEASE(thr->ret);
	sll_platform_free_page(thr,SLL_ROUND_PAGE(sizeof(thread_data_t)+sll_current_vm_config->c_st_sz*sizeof(sll_call_stack_frame_t)+sll_current_vm_config->s_sz));

}



void _scheduler_init(void){
	_scheduler_thread=NULL;
	_scheduler_thread_len=0;
	_scheduler_thread_idx=THREAD_UNKNOWN_INDEX;
	_scheduler_wait_list=NULL;
	_scheduler_wait_list_len=0;
	_scheduler_queue=NULL;
	_scheduler_queue_idx=0;
	_scheduler_queue_len=0;
	_scheduler_lock_list=NULL;
	_scheduler_lock_list_len=0;
}



lock_index_t _scheduler_new_lock(void){
	lock_index_t o=0;
	while (o<_scheduler_lock_list_len){
		if (*(_scheduler_lock_list+o)==THREAD_LOCK_UNUSED){
			break;
		}
		o++;
	}
	if (o==_scheduler_lock_list_len){
		_scheduler_lock_list_len++;
		_scheduler_lock_list=sll_reallocate(_scheduler_lock_list,_scheduler_lock_list_len*sizeof(sll_thread_index_t));
	}
	*(_scheduler_lock_list+o)=THREAD_UNKNOWN_INDEX;
	return o;
}



sll_thread_index_t _scheduler_new_thread(void){
	sll_thread_index_t o=0;
	while (o<_scheduler_thread_len){
		if (!*(_scheduler_thread+o)){
			break;
		}
		o++;
	}
	if (o==_scheduler_thread_len){
		_scheduler_thread_len++;
		_scheduler_thread=sll_reallocate(_scheduler_thread,_scheduler_thread_len*sizeof(thread_data_t*));
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
	*(_scheduler_thread+o)=n;
	if (_scheduler_thread_idx==THREAD_UNKNOWN_INDEX){
		_scheduler_current_thread=n;
		_scheduler_thread_idx=o;
	}
	return o;
}



void _scheduler_queue_next(void){
	_scheduler_current_thread->tm=THREAD_SCHEDULER_INSTRUCTION_COUNT;
	if (!_scheduler_queue_len){
		return;
	}
	sll_thread_index_t tmp=*(_scheduler_queue+_scheduler_queue_idx);
	*(_scheduler_queue+_scheduler_queue_idx)=_scheduler_thread_idx;
	if (!_scheduler_queue_idx){
		_scheduler_queue_idx=_scheduler_queue_len;
	}
	_scheduler_queue_idx--;
	_scheduler_thread_idx=tmp;
	_scheduler_current_thread=*(_scheduler_thread+tmp);
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



void _scheduler_release_lock(sll_integer_t l){
	if (l<0||l>=_scheduler_lock_list_len||*(_scheduler_lock_list+l)==THREAD_LOCK_UNUSED){
		return;
	}
	wait_handle_t wh=THREAD_WAIT_HANDLE_LOCK(l);
	for (wait_list_length_t i=0;i<_scheduler_wait_list_len;i++){
		sll_thread_index_t j=*(_scheduler_wait_list+i);
		if (j==THREAD_UNKNOWN_INDEX){
			continue;
		}
		thread_data_t* k=*(_scheduler_thread+j);
		if (k->wh==wh){
			_scheduler_queue_thread(j);
			*(_scheduler_wait_list+i)=THREAD_UNKNOWN_INDEX;
			*(_scheduler_lock_list+l)=j;
			return;
		}
	}
	*(_scheduler_lock_list+l)=THREAD_UNKNOWN_INDEX;
}



void _scheduler_set_thread(sll_thread_index_t t){
	if (_scheduler_thread_idx==t){
		return;
	}
	if (_scheduler_thread_idx!=THREAD_UNKNOWN_INDEX){
		_scheduler_queue_thread(_scheduler_thread_idx);
	}
	_scheduler_thread_idx=t;
	_scheduler_current_thread=*(_scheduler_thread+t);
}



void _scheduler_terminate_thread(sll_object_t* ret){
	SLL_ASSERT(!_scheduler_current_thread->ret);
	SLL_ACQUIRE(ret);
	_scheduler_current_thread->ret=ret;
	wait_handle_t wh=THREAD_WAIT_HANDLE_THREAD(_scheduler_thread_idx);
	_scheduler_thread_idx=THREAD_UNKNOWN_INDEX;
	for (wait_list_length_t i=0;i<_scheduler_wait_list_len;i++){
		sll_thread_index_t j=*(_scheduler_wait_list+i);
		if (j==THREAD_UNKNOWN_INDEX){
			continue;
		}
		thread_data_t* k=*(_scheduler_thread+j);
		if (k->wh==wh){
			if (_scheduler_thread_idx==THREAD_UNKNOWN_INDEX){
				_scheduler_thread_idx=j;
				_scheduler_current_thread=k;
			}
			else{
				_scheduler_queue_thread(j);
			}
			*(_scheduler_wait_list+i)=THREAD_UNKNOWN_INDEX;
		}
	}
	if (_scheduler_thread_idx==THREAD_UNKNOWN_INDEX){
		_scheduler_thread_idx=_scheduler_queue_pop();
		if (_scheduler_thread_idx!=THREAD_UNKNOWN_INDEX){
			_scheduler_current_thread=*(_scheduler_thread+_scheduler_thread_idx);
		}
	}
}



sll_bool_t _scheduler_wait_lock(sll_integer_t w){
	if (w<0||w>=_scheduler_lock_list_len||*(_scheduler_lock_list+w)==THREAD_LOCK_UNUSED){
		return 0;
	}
	SLL_ASSERT(*(_scheduler_lock_list+w)!=_scheduler_thread_idx);
	if (*(_scheduler_lock_list+w)==THREAD_UNKNOWN_INDEX){
		*(_scheduler_lock_list+w)=_scheduler_thread_idx;
		return 0;
	}
	_scheduler_current_thread->tm=THREAD_SCHEDULER_INSTRUCTION_COUNT;
	_scheduler_current_thread->wh=THREAD_WAIT_HANDLE_LOCK(w);
	_add_wait_list();
	_scheduler_thread_idx=THREAD_UNKNOWN_INDEX;
	return 1;
}



sll_bool_t _scheduler_wait_thread(sll_integer_t w){
	if (w<0||w>=_scheduler_thread_len||!*(_scheduler_thread+w)||w==_scheduler_thread_idx||(*(_scheduler_thread+w))->ret){
		return 0;
	}
	_scheduler_current_thread->tm=THREAD_SCHEDULER_INSTRUCTION_COUNT;
	_scheduler_current_thread->wh=THREAD_WAIT_HANDLE_THREAD(w);
	_add_wait_list();
	_scheduler_thread_idx=THREAD_UNKNOWN_INDEX;
	return 1;
}

#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/memory.h>
#include <sll/platform.h>
#include <sll/types.h>
#include <sll/vm.h>



static thread_data_t** _scheduler_thread;
static sll_thread_index_t _scheduler_thread_next;
static thread_list_length_t _scheduler_thread_len;
static sll_thread_index_t* _scheduler_queue;
static queue_length_t _scheduler_queue_idx;
static queue_length_t _scheduler_queue_len;
static lock_t* _scheduler_lock;
static sll_lock_index_t _scheduler_lock_next;
static lock_list_length_t _scheduler_lock_len;



thread_data_t* _scheduler_current_thread;



__SLL_EXTERNAL sll_thread_index_t sll_current_thread_idx=0;



void _scheduler_deinit(void){
	while (_scheduler_thread_len){
		_scheduler_thread_len--;
		thread_data_t* thr=*(_scheduler_thread+_scheduler_thread_len);
		if (THREAD_IS_UNUSED(thr)){
			continue;
		}
		if (thr->ret){
			SLL_RELEASE(thr->ret);
		}
		sll_platform_free_page(thr,SLL_ROUND_PAGE(sizeof(thread_data_t)+sll_current_vm_config->c_st_sz*sizeof(sll_call_stack_frame_t)+sll_current_vm_config->s_sz));
	}
	sll_deallocate(_scheduler_thread);
	sll_deallocate(_scheduler_queue);
	sll_deallocate(_scheduler_lock);
}



void _scheduler_init(void){
	_scheduler_thread=NULL;
	_scheduler_thread_next=SLL_UNKNOWN_THREAD_INDEX;
	_scheduler_thread_len=0;
	_scheduler_queue=NULL;
	_scheduler_queue_idx=0;
	_scheduler_queue_len=0;
	_scheduler_lock=NULL;
	_scheduler_lock_next=THREAD_LOCK_UNUSED;
	_scheduler_lock_len=0;
	sll_current_thread_idx=SLL_UNKNOWN_THREAD_INDEX;
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
	void* ptr=sll_platform_allocate_page(SLL_ROUND_PAGE(sizeof(thread_data_t)+sll_current_vm_config->c_st_sz*sizeof(sll_call_stack_frame_t)+sll_current_vm_config->s_sz),0);
	thread_data_t* n=ptr;
	n->stack=PTR(ADDR(ptr)+sizeof(thread_data_t)+sll_current_vm_config->c_st_sz*sizeof(sll_call_stack_frame_t));
	n->ii=0;
	n->si=0;
	n->wait=SLL_UNKNOWN_THREAD_INDEX;
	n->ret=NULL;
	n->c_st.dt=PTR(ADDR(ptr)+sizeof(thread_data_t));
	n->c_st.l=0;
	n->tm=THREAD_SCHEDULER_INSTRUCTION_COUNT;
	*(_scheduler_thread+o)=n;
	if (sll_current_thread_idx==SLL_UNKNOWN_THREAD_INDEX){
		_scheduler_current_thread=n;
		sll_current_thread_idx=o;
	}
	return o;
}



void _scheduler_queue_next(void){
	_scheduler_current_thread->tm=THREAD_SCHEDULER_INSTRUCTION_COUNT;
	if (!_scheduler_queue_len){
		return;
	}
	sll_thread_index_t tmp=*(_scheduler_queue+_scheduler_queue_idx);
	*(_scheduler_queue+_scheduler_queue_idx)=sll_current_thread_idx;
	if (!_scheduler_queue_idx){
		_scheduler_queue_idx=_scheduler_queue_len;
	}
	_scheduler_queue_idx--;
	sll_current_thread_idx=tmp;
	_scheduler_current_thread=*(_scheduler_thread+tmp);
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
	return o;
}



void _scheduler_queue_thread(sll_thread_index_t t){
	_scheduler_queue_len++;
	_scheduler_queue=sll_reallocate(_scheduler_queue,_scheduler_queue_len*sizeof(sll_thread_index_t));
	*(_scheduler_queue+_scheduler_queue_len-1)=t;
}



void _scheduler_set_thread(sll_thread_index_t t){
	if (sll_current_thread_idx==t){
		return;
	}
	if (sll_current_thread_idx!=SLL_UNKNOWN_THREAD_INDEX){
		SLL_UNIMPLEMENTED();
	}
	sll_current_thread_idx=t;
	_scheduler_current_thread=*(_scheduler_thread+t);
}



void _scheduler_terminate_thread(sll_object_t* ret){
	SLL_ASSERT(!_scheduler_current_thread->ret);
	SLL_ACQUIRE(ret);
	_scheduler_current_thread->ret=ret;
	sll_current_thread_idx=_scheduler_current_thread->wait;
	if (sll_current_thread_idx==SLL_UNKNOWN_THREAD_INDEX){
		sll_current_thread_idx=_scheduler_queue_pop();
		if (sll_current_thread_idx!=SLL_UNKNOWN_THREAD_INDEX){
			_scheduler_current_thread=*(_scheduler_thread+sll_current_thread_idx);
		}
		return;
	}
	_scheduler_current_thread=*(_scheduler_thread+sll_current_thread_idx);
	while (_scheduler_current_thread->nxt!=SLL_UNKNOWN_THREAD_INDEX){
		_scheduler_queue_thread(sll_current_thread_idx);
		sll_current_thread_idx=_scheduler_current_thread->nxt;
		_scheduler_current_thread=*(_scheduler_thread+sll_current_thread_idx);
	}
}



sll_bool_t _scheduler_wait_lock(sll_integer_t w){
	if (w<0||w>=_scheduler_lock_len||(_scheduler_lock+w)->lock==THREAD_LOCK_UNUSED){
		return 0;
	}
	SLL_ASSERT((_scheduler_lock+w)->lock!=sll_current_thread_idx);
	if ((_scheduler_lock+w)->lock==SLL_UNKNOWN_THREAD_INDEX){
		(_scheduler_lock+w)->lock=sll_current_thread_idx;
		(_scheduler_lock+w)->first=SLL_UNKNOWN_THREAD_INDEX;
		return 0;
	}
	if ((_scheduler_lock+w)->first==SLL_UNKNOWN_THREAD_INDEX){
		(_scheduler_lock+w)->first=sll_current_thread_idx;
	}
	else{
		(_scheduler_lock+w)->last->nxt=sll_current_thread_idx;
	}
	(_scheduler_lock+w)->last=_scheduler_current_thread;
	_scheduler_current_thread->nxt=SLL_UNKNOWN_THREAD_INDEX;
	sll_current_thread_idx=SLL_UNKNOWN_THREAD_INDEX;
	return 1;
}



sll_bool_t _scheduler_wait_thread(sll_integer_t w){
	if (w<0||w>=_scheduler_thread_len||!*(_scheduler_thread+w)||w==sll_current_thread_idx||(*(_scheduler_thread+w))->ret){
		return 0;
	}
	_scheduler_current_thread->tm=THREAD_SCHEDULER_INSTRUCTION_COUNT;
	thread_data_t* thr=*(_scheduler_thread+w);
	_scheduler_current_thread->nxt=thr->wait;
	thr->wait=sll_current_thread_idx;
	sll_current_thread_idx=SLL_UNKNOWN_THREAD_INDEX;
	return 1;
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



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_thread_index_t sll_create_thread(sll_integer_t fn,sll_object_t*const* al,sll_arg_count_t all){
	sll_thread_index_t tid=_init_thread_stack(fn,al,all);
	_scheduler_queue_thread(tid);
	return tid;
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



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_delete_thread(sll_thread_index_t t){
	if (t>=_scheduler_thread_len){
		return 0;
	}
	thread_data_t* thr=*(_scheduler_thread+t);
	if (THREAD_IS_UNUSED(thr)){
		return 0;
	}
	SLL_ASSERT(thr->ret);
	*(_scheduler_thread+t)=THREAD_NEXT_UNUSED(_scheduler_thread_next);
	_scheduler_thread_next=t;
	SLL_RELEASE(thr->ret);
	sll_platform_free_page(thr,SLL_ROUND_PAGE(sizeof(thread_data_t)+sll_current_vm_config->c_st_sz*sizeof(sll_call_stack_frame_t)+sll_current_vm_config->s_sz));
	return 1;
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
	_scheduler_queue_thread((_scheduler_lock+l)->lock);
	return 1;
}

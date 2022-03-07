#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/error.h>
#include <sll/file.h>
#include <sll/memory.h>
#include <sll/object.h>
#include <sll/platform.h>
#include <sll/scheduler.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>



static event_data_t* _io_dispatcher_event;
static event_list_length_t _io_dispatcher_event_next;
static event_list_length_t _io_dispatcher_event_cnt;
static event_list_length_t _io_dispatcher_event_len;



static sll_thread_index_t _restart_thread(event_list_length_t idx){
	_io_dispatcher_event_cnt--;
	event_data_t* evt=_io_dispatcher_event+idx;
	SLL_ASSERT(evt->f);
	sll_object_t* o=NULL;
	if (evt->sz){
		o=SLL_CREATE();
		o->t=SLL_OBJECT_TYPE_STRING;
		sll_string_create(evt->sz,&(o->dt.s));
		sll_error_t err;
		sll_size_t sz=sll_file_read(evt->f,o->dt.s.v,evt->sz,&err);
		if (!sz&&err!=SLL_NO_ERROR){
			sll_free_string(&(o->dt.s));
			o->t=SLL_OBJECT_TYPE_INT;
			o->dt.i=err;
		}
		else{
			sll_string_decrease(&(o->dt.s),(sll_string_length_t)sz);
			sll_string_calculate_checksum(&(o->dt.s));
		}
	}
	else{
		sll_error_t err;
		sll_read_char_t chr=sll_file_read_char(evt->f,&err);
		o=(chr==SLL_END_OF_DATA?(err==SLL_NO_ERROR?SLL_ACQUIRE_STATIC_INT(0):SLL_FROM_INT(~err)):SLL_FROM_CHAR(chr));
	}
	sll_thread_index_t tid=evt->tid;
	thread_data_t* thr=_scheduler_get_thread(tid);
	*(thr->stack+thr->si)=o;
	thr->si++;
	evt->f=NULL;
	EVENT_SET_NEXT_ID(evt,_io_dispatcher_event_next);
	_io_dispatcher_event_next=idx;
	return tid;
}



void _io_dispatcher_deinit(void){
	sll_deallocate(_io_dispatcher_event);
}



void _io_dispatcher_init(void){
	_io_dispatcher_event=NULL;
	_io_dispatcher_event_next=EVENT_UNUSED;
	_io_dispatcher_event_cnt=0;
	_io_dispatcher_event_len=0;
}



void _io_dispatcher_queue(sll_file_t* f,sll_string_length_t sz){
	_io_dispatcher_event_cnt++;
	event_list_length_t i=_io_dispatcher_event_next;
	if (_io_dispatcher_event_next==EVENT_UNUSED){
		i=_io_dispatcher_event_len;
		_io_dispatcher_event_len++;
		_io_dispatcher_event=sll_reallocate(_io_dispatcher_event,_io_dispatcher_event_len*sizeof(event_data_t));
	}
	else{
		_io_dispatcher_event_next=EVENT_GET_NEXT_ID(_io_dispatcher_event+i);
	}
	(_io_dispatcher_event+i)->f=f;
	(_io_dispatcher_event+i)->tid=sll_current_thread_index;
	(_io_dispatcher_event+i)->sz=sz;
	_scheduler_current_thread->ii++;
	_scheduler_current_thread->st=THREAD_STATE_WAIT_IO;
	sll_current_thread_index=SLL_UNKNOWN_THREAD_INDEX;
}



sll_thread_index_t _io_dispatcher_wait(void){
	if (!_io_dispatcher_event_cnt){
		return SLL_UNKNOWN_THREAD_INDEX;
	}
	event_list_length_t i=0;
	while (1){
		event_data_t* evt=_io_dispatcher_event+i;
		if (evt->f&&sll_platform_file_data_available(evt->f->dt.fl.fd)){
			return _restart_thread(i);
		}
		i++;
		if (i==_io_dispatcher_event_len){
			i=0;
		}
	}
}

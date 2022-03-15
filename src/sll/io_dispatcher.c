#include <sll/_internal/common.h>
#include <sll/_internal/dispatcher.h>
#include <sll/_internal/scheduler.h>
#include <sll/common.h>
#include <sll/error.h>
#include <sll/file.h>
#include <sll/memory.h>
#include <sll/object.h>
#include <sll/platform/_dispatcher.h>
#include <sll/platform/file.h>
#include <sll/platform/thread.h>
#include <sll/thread.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>



static volatile sll_bool_t _io_dispatcher_end;
static void* _io_dispatcher_wait_handle;
static event_data_t* _io_dispatcher_event;
static event_list_length_t _io_dispatcher_event_len;
static sll_internal_thread_index_t _io_dispatcher_thread;
static raw_event_data_t* _io_dispatcher_raw_event;
static volatile dispatched_thread_t _io_dispatcher_restarted_thread;



static sll_thread_index_t _restart_thread(event_list_length_t idx){
	event_data_t* evt=_io_dispatcher_event+idx;
	SLL_ASSERT(evt->f);
	sll_object_t* o=NULL;
	if (evt->sz){
		o=sll_create_object();
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
		o=(chr==SLL_END_OF_DATA?(err==SLL_NO_ERROR?SLL_ACQUIRE_STATIC_INT(0):sll_int_to_object(~err)):SLL_FROM_CHAR(chr));
	}
	sll_thread_index_t tid=evt->tid;
	thread_data_t* thr=_scheduler_get_thread(tid);
	*(thr->stack+thr->si)=o;
	thr->si++;
	for (event_list_length_t i=idx+1;i<_io_dispatcher_event_len;i++){
		*(_io_dispatcher_event+i-1)=*(_io_dispatcher_event+i);
		*(_io_dispatcher_raw_event+i)=*(_io_dispatcher_raw_event+i+1);
	}
	_io_dispatcher_event_len--;
	_io_dispatcher_event=sll_reallocate(_io_dispatcher_event,_io_dispatcher_event_len*sizeof(event_data_t));
	_io_dispatcher_raw_event=sll_reallocate(_io_dispatcher_raw_event,(_io_dispatcher_event_len+1)*sizeof(raw_event_data_t));
	return tid;
}



static void _poll_thread(void* arg){
	while (1){
		event_list_length_t i=_platform_poll_events(_io_dispatcher_raw_event,_io_dispatcher_wait_handle,_io_dispatcher_event_len+1);
		if (_io_dispatcher_end){
			return;
		}
		if (i==_io_dispatcher_event_len+1){
			continue;
		}
		if (!i){
			_platform_wait_for_dispatch(_io_dispatcher_raw_event);
			continue;
		}
		sll_thread_index_t tid=_restart_thread(i-1);
		if (_io_dispatcher_restarted_thread.tid==SLL_UNKNOWN_THREAD_INDEX){
			_io_dispatcher_restarted_thread.tid=tid;
			_platform_notify_dispatch(&_io_dispatcher_restarted_thread);
		}
		else{
			_scheduler_queue_thread(tid);
		}
	}
}



void _io_dispatcher_deinit(void){
	_io_dispatcher_end=1;
	_platform_poll_stop(_io_dispatcher_raw_event,NULL);
	if (!sll_platform_join_thread(_io_dispatcher_thread)){
		SLL_UNREACHABLE();
	}
	_platform_deinit_io_dispatcher(_io_dispatcher_raw_event,_io_dispatcher_wait_handle,&_io_dispatcher_restarted_thread);
	sll_deallocate(_io_dispatcher_event);
	sll_deallocate(_io_dispatcher_raw_event);
}



void _io_dispatcher_init(void){
	_io_dispatcher_end=0;
	_io_dispatcher_event=NULL;
	_io_dispatcher_event_len=0;
	_io_dispatcher_raw_event=sll_allocate(sizeof(raw_event_data_t));
	_platform_init_io_dispatcher(_io_dispatcher_raw_event,&_io_dispatcher_wait_handle,&_io_dispatcher_restarted_thread);
	_io_dispatcher_thread=sll_platform_start_thread(_poll_thread,NULL);
	SLL_ASSERT(_io_dispatcher_thread!=SLL_UNKNOWN_INTERNAL_THREAD_INDEX);
}



void _io_dispatcher_queue(sll_file_t* f,sll_string_length_t sz){
	_platform_poll_stop(_io_dispatcher_raw_event,&_io_dispatcher_wait_handle);
	event_list_length_t i=_io_dispatcher_event_len;
	_io_dispatcher_event_len++;
	_io_dispatcher_event=sll_reallocate(_io_dispatcher_event,_io_dispatcher_event_len*sizeof(event_data_t));
	_io_dispatcher_raw_event=sll_reallocate(_io_dispatcher_raw_event,(_io_dispatcher_event_len+1)*sizeof(raw_event_data_t));
	(_io_dispatcher_event+i)->f=f;
	(_io_dispatcher_event+i)->tid=sll_current_thread_index;
	(_io_dispatcher_event+i)->sz=sz;
	INIT_RAW_EVENT(_io_dispatcher_raw_event+i+1,f->dt.fl.fd);
	_platform_poll_start(_io_dispatcher_raw_event);
	_scheduler_current_thread->ii++;
	_scheduler_current_thread->st=THREAD_STATE_WAIT_IO;
	sll_current_thread_index=SLL_UNKNOWN_THREAD_INDEX;
}



sll_thread_index_t _io_dispatcher_wait(void){
	if (!_io_dispatcher_event_len){
		return SLL_UNKNOWN_THREAD_INDEX;
	}
	return _platform_wait_notify_dispatch(&_io_dispatcher_restarted_thread);
}

#include <sll/_internal/common.h>
#include <sll/_internal/dispatcher.h>
#include <sll/_internal/scheduler.h>
#include <sll/_internal/static_object.h>
#include <sll/common.h>
#include <sll/error.h>
#include <sll/file.h>
#include <sll/gc.h>
#include <sll/memory.h>
#include <sll/object.h>
#include <sll/platform/_dispatcher.h>
#include <sll/platform/file.h>
#include <sll/platform/thread.h>
#include <sll/scheduler.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/thread.h>
#include <sll/types.h>



static volatile sll_bool_t _io_dispatcher_end;
static void* _io_dispatcher_wait_handle;
static event_data_t* _io_dispatcher_event;
static event_list_length_t _io_dispatcher_event_len;
static sll_internal_thread_index_t _io_dispatcher_thread;
static raw_event_data_t* _io_dispatcher_raw_event;



static sll_thread_index_t _restart_thread(event_list_length_t idx){
	event_data_t* evt=_io_dispatcher_event+idx;
	SLL_ASSERT(evt->file);
	sll_object_t out=NULL;
	if (evt->size==SLL_MAX_STRING_LENGTH){
		sll_error_t err;
		sll_read_char_t chr=sll_file_read_char(evt->file,&err);
		out=(chr==SLL_END_OF_DATA?(err==SLL_NO_ERROR?SLL_ACQUIRE_STATIC_INT(0):sll_int_to_object(~err)):SLL_FROM_CHAR(chr));
	}
	else if (!evt->size){
		SLL_UNIMPLEMENTED();
	}
	else{
		sll_string_t buffer;
		sll_string_create(evt->size,&buffer);
		sll_error_t err;
		sll_size_t size=sll_file_read(evt->file,buffer.data,evt->size,&err);
		if (!size&&err!=SLL_NO_ERROR){
			sll_free_string(&buffer);
			out=sll_int_to_object(err);
		}
		else{
			sll_string_decrease(&buffer,(sll_string_length_t)size);
			sll_string_calculate_checksum(&buffer);
			out=STRING_TO_OBJECT_NOCOPY(&buffer);
		}
	}
	sll_thread_index_t thread_index=evt->thread_index;
	thread_data_t* thread=_thread_get(thread_index);
	*(thread->stack+thread->stack_index)=out;
	thread->stack_index++;
	for (event_list_length_t i=idx+1;i<_io_dispatcher_event_len;i++){
		*(_io_dispatcher_event+i-1)=*(_io_dispatcher_event+i);
		*(_io_dispatcher_raw_event+i)=*(_io_dispatcher_raw_event+i+1);
	}
	_io_dispatcher_event_len--;
	_io_dispatcher_event=sll_reallocate(_io_dispatcher_event,_io_dispatcher_event_len*sizeof(event_data_t));
	_io_dispatcher_raw_event=sll_reallocate(_io_dispatcher_raw_event,(_io_dispatcher_event_len+1)*sizeof(raw_event_data_t));
	return thread_index;
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
		_scheduler_queue_thread(tid);
	}
}



void _io_dispatcher_deinit(void){
	_io_dispatcher_end=1;
	_platform_poll_stop(_io_dispatcher_raw_event,NULL);
	SLL_CRITICAL_ERROR(sll_platform_join_thread(_io_dispatcher_thread));
	_platform_deinit_io_dispatcher(_io_dispatcher_raw_event,_io_dispatcher_wait_handle);
	sll_deallocate(_io_dispatcher_event);
	sll_deallocate(_io_dispatcher_raw_event);
}



void _io_dispatcher_init(void){
	_io_dispatcher_end=0;
	_io_dispatcher_event=NULL;
	_io_dispatcher_event_len=0;
	_io_dispatcher_raw_event=sll_allocate(sizeof(raw_event_data_t));
	_platform_init_io_dispatcher(_io_dispatcher_raw_event,&_io_dispatcher_wait_handle);
	_io_dispatcher_thread=sll_platform_start_thread(_poll_thread,NULL,NULL);
	SLL_ASSERT(_io_dispatcher_thread!=SLL_UNKNOWN_INTERNAL_THREAD_INDEX);
}



void _io_dispatcher_queue(sll_file_t* file,sll_string_length_t size){
	_platform_poll_stop(_io_dispatcher_raw_event,&_io_dispatcher_wait_handle);
	event_list_length_t i=_io_dispatcher_event_len;
	_io_dispatcher_event_len++;
	_io_dispatcher_event=sll_reallocate(_io_dispatcher_event,_io_dispatcher_event_len*sizeof(event_data_t));
	_io_dispatcher_raw_event=sll_reallocate(_io_dispatcher_raw_event,(_io_dispatcher_event_len+1)*sizeof(raw_event_data_t));
	(_io_dispatcher_event+i)->file=file;
	(_io_dispatcher_event+i)->thread_index=_scheduler_current_thread_index;
	(_io_dispatcher_event+i)->size=size;
	INIT_RAW_EVENT(_io_dispatcher_raw_event+i+1,file->data.file.source.file.fd);
	_platform_poll_start(_io_dispatcher_raw_event);
	_scheduler_current_thread->state=THREAD_STATE_WAIT_IO;
	_scheduler_current_thread_index=SLL_UNKNOWN_THREAD_INDEX;
}

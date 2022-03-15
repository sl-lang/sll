#include <windows.h>
#include <sll/_internal/common.h>
#include <sll/_internal/dispatcher.h>
#include <sll/common.h>
#include <sll/platform/file.h>
#include <sll/thread.h>
#include <sll/types.h>



static __SLL_FORCE_INLINE void _platform_deinit_io_dispatcher(raw_event_data_t* r_dt,void* wait){
	CloseHandle(*r_dt);
	CloseHandle(wait);
}



static __SLL_FORCE_INLINE void _platform_init_io_dispatcher(raw_event_data_t* r_dt,void** wait){
	*r_dt=CreateEventA(NULL,TRUE,FALSE,NULL);
	*wait=CreateEventA(NULL,TRUE,FALSE,NULL);
}



static __SLL_FORCE_INLINE event_list_length_t _platform_poll_events(raw_event_data_t* dt,void* wait,event_list_length_t cnt){
	if (cnt>MAXIMUM_WAIT_OBJECTS){
		SLL_UNIMPLEMENTED();
	}
	DWORD o=WaitForMultipleObjects(cnt,dt,FALSE,INFINITE);
	if (o==WAIT_FAILED){
		return cnt;
	}
	o-=WAIT_OBJECT_0;
	SLL_ASSERT(o<cnt);
	if (!o){
		ResetEvent(*dt);
		SetEvent(wait);
		return 0;
	}
	return (sll_platform_file_data_available(*(dt+o))?o:cnt);
}



static __SLL_FORCE_INLINE void _platform_poll_start(raw_event_data_t* dt){
	SetEvent(*dt);
}



static __SLL_FORCE_INLINE void _platform_poll_stop(raw_event_data_t* dt,void** wait){
	SetEvent(*dt);
	if (!wait){
		return;
	}
	if (WaitForSingleObject(*wait,INFINITE)!=WAIT_OBJECT_0){
		SLL_UNIMPLEMENTED();
	}
	ResetEvent(*wait);
}



static __SLL_FORCE_INLINE void _platform_wait_for_dispatch(raw_event_data_t* dt){
	if (WaitForSingleObject(*dt,INFINITE)!=WAIT_OBJECT_0){
		SLL_UNIMPLEMENTED();
	}
	ResetEvent(*dt);
}

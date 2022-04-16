#ifndef __SLL__INTERNAL_DISPATCHER_H__
#define __SLL__INTERNAL_DISPATCHER_H__ 1
#ifndef __SLL_BUILD_WINDOWS
#include <poll.h>
#endif
#include <sll/_internal/common.h>
#include <sll/_size_types.h>
#include <sll/types.h>



#ifdef __SLL_BUILD_WINDOWS
#define INIT_RAW_EVENT(r,f) (*(r)=(f))
#else
#define INIT_RAW_EVENT(r,f) \
	do{ \
		(r)->fd=(int)ADDR((f)); \
		(r)->events=POLLIN; \
	} while (0)
#endif



typedef __SLL_U32 event_list_length_t;



typedef struct _EVENT_DATA{
	sll_file_t* f;
	sll_thread_index_t tid;
	sll_string_length_t sz;
} event_data_t;



#ifdef __SLL_BUILD_WINDOWS
typedef void* raw_event_data_t;
#else
typedef struct pollfd raw_event_data_t;
#endif



void _io_dispatcher_deinit(void);



void _io_dispatcher_init(void);



void _io_dispatcher_queue(sll_file_t* f,sll_string_length_t sz);



#endif

#ifndef __SLL__INTERNAL_DISPATCHER_H__
#define __SLL__INTERNAL_DISPATCHER_H__ 1
#ifndef __SLL_BUILD_WINDOWS
#include <poll.h>
#endif
#include <sll/_internal/common.h>
#include <sll/_size_types.h>
#include <sll/file.h>
#include <sll/string.h>
#include <sll/thread.h>
#include <sll/types.h>



#ifdef __SLL_BUILD_WINDOWS
#define INIT_RAW_EVENT(raw,fd_) (*(raw)=(fd_))
#else
#define INIT_RAW_EVENT(raw,fd_) \
	do{ \
		(raw)->fd=(int)ADDR((fd_)); \
		(raw)->events=POLLIN; \
	} while (0)
#endif



typedef __SLL_U32 event_list_length_t;



typedef struct _EVENT_DATA{
	sll_file_t* file;
	sll_thread_index_t thread_index;
	sll_string_length_t size;
} event_data_t;



#ifdef __SLL_BUILD_WINDOWS
typedef void* raw_event_data_t;
#else
typedef struct pollfd raw_event_data_t;
#endif



void _io_dispatcher_deinit(void);



void _io_dispatcher_init(void);



void _io_dispatcher_queue(sll_file_t* file,sll_string_length_t sz);



#endif

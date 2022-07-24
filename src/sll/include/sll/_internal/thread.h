#ifndef __SLL__INTERNAL_THREAD_H__
#define __SLL__INTERNAL_THREAD_H__ 1
#include <sll/_internal/common.h>
#include <sll/_internal/sandbox.h>
#include <sll/_size_types.h>
#include <sll/assembly.h>
#include <sll/container.h>
#include <sll/gc.h>
#include <sll/sandbox.h>
#include <sll/thread.h>
#include <sll/types.h>
#include <sll/vm.h>



#define THREAD_SCHEDULER_INSTRUCTION_COUNT 10
#define THREAD_SIZE SLL_ROUND_PAGE(sizeof(thread_data_t)+sll_current_vm_config->call_stack_size*sizeof(sll_call_stack_frame_t)+(sll_current_vm_config->stack_size+sll_current_runtime_data->assembly_data->tls_variable_count)*sizeof(sll_object_t))

#define THREAD_ALLOCATOR_CACHE_POOL_SIZE 32

#define THREAD_STATE_INITIALIZED 0
#define THREAD_STATE_RUNNING 1
#define THREAD_STATE_QUEUED 2
#define THREAD_STATE_WAIT_BARRIER 3
#define THREAD_STATE_WAIT_IO 4
#define THREAD_STATE_WAIT_LOCK 5
#define THREAD_STATE_WAIT_SEMAPHORE 6
#define THREAD_STATE_WAIT_THREAD 7
#define THREAD_STATE_TERMINATED 8
#define THREAD_STATE_UNDEFINED 255

#define THREAD_FLAG_SUSPENDED 1
#define THREAD_FLAG_DELETE 2
#define THREAD_FLAG_NO_AUDIT_TERMINATE 4


typedef __SLL_U8 thread_flags_t;



typedef __SLL_U8 thread_state_t;



typedef __SLL_U32 thread_list_length_t;



typedef struct _THREAD_DATA{
	sll_object_t* stack;
	sll_object_t* tls;
	sll_instruction_index_t instruction_index;
	sll_stack_offset_t stack_index;
	sll_thread_index_t next;
	sll_thread_index_t wait;
	sll_object_t return_value;
	sll_call_stack_t call_stack;
	sll_sandbox_flags_t sandbox;
	sll_char_t time_quantum;
	thread_state_t state;
	thread_flags_t flags;
	const sll_assembly_instruction_t* _last_instruction;
} thread_data_t;



extern volatile thread_list_length_t _thread_active_count;
extern sll_handle_container_t _thread_data;



thread_data_t* _thread_get(sll_thread_index_t thread_index);



sll_thread_index_t _thread_new(void);



void _thread_terminate(sll_object_t return_value);



sll_bool_t _thread_wait(sll_integer_t thread_index);



void _thread_deinit(void);



void _thread_init(void);



#endif

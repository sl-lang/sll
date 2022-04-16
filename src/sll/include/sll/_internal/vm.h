#ifndef __SLL__INTERNAL_VM_H__
#define __SLL__INTERNAL_VM_H__ 1
#include <sll/_internal/thread.h>
#include <sll/types.h>



#define EXECUTE_FUNCTION_NO_AUDIT_TERMINATE SLL_EXECUTE_FUNCTION_RESERVED0



__SLL_API_CALL void _call_api_func(sll_internal_function_pointer_t fn,bitmap_t* regs,const void* data,void* ret,sll_object_t*const* al,sll_arg_count_t all);



void _call_function(thread_data_t* thr,sll_function_index_t fn,sll_arg_count_t ac,sll_bool_t fr);



#endif

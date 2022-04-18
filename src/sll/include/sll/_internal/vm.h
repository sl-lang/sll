#ifndef __SLL__INTERNAL_VM_H__
#define __SLL__INTERNAL_VM_H__ 1
#include <sll/_internal/thread.h>
#include <sll/types.h>



#define EXECUTE_FUNCTION_NO_AUDIT_TERMINATE SLL_EXECUTE_FUNCTION_RESERVED0



void _call_function(thread_data_t* thr,sll_function_index_t fn,sll_arg_count_t ac,sll_bool_t fr);



#endif

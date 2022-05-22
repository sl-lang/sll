#include <sll/_internal/scheduler.h>
#include <sll/common.h>
#include <sll/sandbox.h>
#include <sll/scheduler.h>
#include <sll/thread.h>
#include <sll/types.h>



sll_sandbox_flags_t _sandbox_flags=0;



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_get_sandbox_flag(sll_sandbox_flag_t flag){
	return !!((_scheduler_current_thread_index==SLL_UNKNOWN_THREAD_INDEX?_sandbox_flags:_scheduler_current_thread->sandbox)&(1ull<<flag));
}



__SLL_EXTERNAL sll_sandbox_flags_t sll_get_sandbox_flags(void){
	return (_scheduler_current_thread_index==SLL_UNKNOWN_THREAD_INDEX?_sandbox_flags:_scheduler_current_thread->sandbox);
}



__SLL_EXTERNAL void sll_set_sandbox_flag(sll_sandbox_flag_t flag){
	if (flag>SLL_MAX_SANDBOX_FLAG){
		return;
	}
	if (_scheduler_current_thread_index==SLL_UNKNOWN_THREAD_INDEX){
		_sandbox_flags|=1ull<<flag;
	}
	else{
		_scheduler_current_thread->sandbox|=1ull<<flag;
	}
}

#include <sll/_internal/scheduler.h>
#include <sll/api/vm.h>
#include <sll/array.h>
#include <sll/audit.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/new_object.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/thread.h>
#include <sll/types.h>
#include <sll/vm.h>



__SLL_EXTERNAL __SLL_API_CALL void sll_api_error_get_call_stack(sll_call_stack_size_t pop,sll_thread_index_t tid,sll_array_t* out){
	sll_audit(SLL_CHAR("sll.error.backtrace"),SLL_CHAR("u"),tid);
	const sll_call_stack_t* c_st=sll_thread_get_call_stack(tid);
	if (!c_st||pop>c_st->l){
		SLL_INIT_ARRAY(out);
		return;
	}
	sll_object_t* o=sll_new_object(SLL_CHAR("{S#(hh)}"),c_st->dt,c_st->l-pop,sizeof(const sll_call_stack_frame_t),SLL_OFFSETOF(sll_call_stack_frame_t,nm),sll_instruction_to_location,SLL_OFFSETOF(sll_call_stack_frame_t,_ii),SLL_OFFSETOF(sll_call_stack_frame_t,_ii),SLL_OFFSETOF(sll_call_stack_frame_t,_s));
	*out=o->dt.array;
	SLL_CRITICAL(sll_destroy_object(o));
}

#include <sll/_internal/scheduler.h>
#include <sll/api/vm.h>
#include <sll/array.h>
#include <sll/audit.h>
#include <sll/common.h>
#include <sll/error.h>
#include <sll/gc.h>
#include <sll/new_object.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/thread.h>
#include <sll/types.h>
#include <sll/vm.h>



static sll_object_t* _wrapper(sll_instruction_index_t* ii){
	return sll_instruction_to_location(*ii);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_error_get_call_stack(sll_call_stack_size_t pop,sll_thread_index_t thread_index,sll_array_t* out){
	sll_audit(SLL_CHAR("sll.error.backtrace"),SLL_CHAR("u"),thread_index);
	const sll_call_stack_t* c_st=sll_thread_get_call_stack(thread_index);
	if (!c_st||pop>c_st->length){
		SLL_INIT_ARRAY(out);
		return;
	}
	sll_object_t* o=sll_new_object(SLL_CHAR("{#(hh)}"),c_st->data,c_st->length-pop,sizeof(const sll_call_stack_frame_t),_wrapper,SLL_OFFSETOF(sll_call_stack_frame_t,_instruction_index),SLL_OFFSETOF(sll_call_stack_frame_t,_instruction_index),SLL_OFFSETOF(sll_call_stack_frame_t,_stack_offset));
	*out=o->data.array;
	SLL_CRITICAL(sll_destroy_object(o));
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_error_get_error_string(sll_error_t err,sll_string_t* out){
	SLL_INIT_STRING(out);
	if (SLL_ERROR_GET_TYPE(err)!=SLL_ERROR_FLAG_SLL||SLL_ERROR_GET_VALUE(err)!=SLL_ERROR_STRING){
		return;
	}
	const sll_char_t* ptr=sll_error_get_string_pointer(err);
	if (ptr){
		sll_string_from_pointer(ptr,out);
	}
}

#include <sll/_internal/api.h>
#include <sll/_internal/scheduler.h>
#include <sll/api.h>
#include <sll/api/vm.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/new_object.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/thread.h>
#include <sll/types.h>
#include <sll/vm.h>



__API_FUNC(error_get_call_stack){
	if (a<0){
		a=0;
	}
	const sll_call_stack_t* c_st=sll_thread_get_call_stack((b<0?_scheduler_current_thread_index:(sll_thread_index_t)b));
	if (!c_st||a>c_st->l){
		SLL_INIT_ARRAY(out);
		return;
	}
	sll_array_create(c_st->l-((sll_array_length_t)a),out);
	for (sll_call_stack_size_t i=0;i<c_st->l-a;i++){
		const sll_call_stack_frame_t* fr=c_st->dt+i;
		out->v[i]=sll_new_object(SLL_CHAR("[SN[hh]]"),fr->nm,sll_instruction_to_location(fr->_ii),fr->_ii,fr->_s);
	}
}

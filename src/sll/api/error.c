#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/api/vm.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/object.h>
#include <sll/scheduler.h>
#include <sll/static_object.h>
#include <sll/types.h>
#include <sll/vm.h>



__API_FUNC(error_get_call_stack){
	if (a<0){
		a=0;
	}
	const sll_call_stack_t* c_st=sll_get_call_stack((b<0?sll_current_thread_index:(sll_thread_index_t)b));
	if (!c_st||a>c_st->l){
		SLL_INIT_ARRAY(out);
		return;
	}
	sll_array_create(c_st->l-((sll_array_length_t)a),out);
	for (sll_call_stack_size_t i=0;i<c_st->l-a;i++){
		const sll_call_stack_frame_t* fr=c_st->dt+i;
		sll_object_t* nm=SLL_CREATE();
		nm->t=SLL_OBJECT_TYPE_STRING;
		sll_string_from_pointer(fr->nm,&(nm->dt.s));
		sll_object_t* r_dt=SLL_CREATE();
		r_dt->t=SLL_OBJECT_TYPE_ARRAY;
		sll_array_create(2,&(r_dt->dt.a));
		r_dt->dt.a.v[0]=SLL_FROM_INT(fr->_ii);
		r_dt->dt.a.v[1]=SLL_FROM_INT(fr->_s);
		sll_object_t* fr_o=SLL_CREATE();
		fr_o->t=SLL_OBJECT_TYPE_ARRAY;
		sll_array_create(3,&(fr_o->dt.a));
		fr_o->dt.a.v[0]=nm;
		fr_o->dt.a.v[1]=sll_instruction_to_location(fr->_ii);
		fr_o->dt.a.v[2]=r_dt;
		out->v[i]=fr_o;
	}
}

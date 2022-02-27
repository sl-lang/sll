#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/location.h>
#include <sll/object.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/vm.h>



__API_FUNC(error_get_backtrace){
	const sll_call_stack_t* c_st=sll_get_call_stack();
	sll_array_create(c_st->l,out);
	for (sll_call_stack_size_t i=0;i<c_st->l;i++){
		const sll_call_stack_frame_t* k=c_st->dt+i;
		sll_string_index_t fp_i;
		sll_string_index_t fn_i;
		sll_file_offset_t ln=sll_get_location(sll_current_runtime_data->a_dt,k->_ii,&fp_i,&fn_i);
		sll_object_t* fp=SLL_CREATE();
		fp->t=SLL_OBJECT_TYPE_STRING;
		sll_string_clone(sll_current_runtime_data->a_dt->st.dt+fp_i,&(fp->dt.s));
		sll_object_t* fn=SLL_CREATE();
		fn->t=SLL_OBJECT_TYPE_STRING;
		if (fn_i==SLL_MAX_STRING_INDEX){
			sll_string_from_pointer(SLL_CHAR("@code"),&(fn->dt.s));
		}
		else{
			sll_string_clone(sll_current_runtime_data->a_dt->st.dt+fn_i,&(fn->dt.s));
		}
		sll_object_t* loc=SLL_CREATE();
		loc->t=SLL_OBJECT_TYPE_ARRAY;
		sll_array_create(3,&(loc->dt.a));
		loc->dt.a.v[0]=fp;
		loc->dt.a.v[1]=SLL_FROM_INT(ln);
		loc->dt.a.v[2]=fn;
		out->v[i]=loc;
	}
}

#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/api/file.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/location.h>
#include <sll/object.h>
#include <sll/scheduler.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/vm.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_instruction_to_location(sll_instruction_index_t ii){
	sll_string_index_t fp_i;
	sll_string_index_t fn_i;
	sll_file_offset_t ln=sll_get_location(sll_current_runtime_data->a_dt,ii,&fp_i,&fn_i);
	sll_object_t* fp=sll_create_object();
	fp->t=SLL_OBJECT_TYPE_STRING;
	sll_string_clone(sll_current_runtime_data->a_dt->st.dt+fp_i,&(fp->dt.s));
	sll_object_t* fn=sll_create_object();
	fn->t=SLL_OBJECT_TYPE_STRING;
	if (fn_i==SLL_MAX_STRING_INDEX){
		sll_string_from_pointer(SLL_CHAR("@code"),&(fn->dt.s));
	}
	else{
		sll_string_clone(sll_current_runtime_data->a_dt->st.dt+fn_i,&(fn->dt.s));
	}
	sll_object_t* o=sll_create_object();
	o->t=SLL_OBJECT_TYPE_ARRAY;
	sll_array_create(3,&(o->dt.a));
	o->dt.a.v[0]=fp;
	o->dt.a.v[1]=sll_int_to_object(ln);
	o->dt.a.v[2]=fn;
	return o;
}



__API_FUNC_DECL(vm_get_config){
	sll_array_create(5,out);
	out->v[0]=sll_int_to_object(sll_current_vm_config->s_sz);
	out->v[1]=sll_int_to_object(sll_current_vm_config->c_st_sz);
	out->v[2]=sll_int_to_object(sll_file_to_handle(sll_current_vm_config->in));
	out->v[3]=sll_int_to_object(sll_file_to_handle(sll_current_vm_config->out));
	out->v[4]=sll_int_to_object(sll_file_to_handle(sll_current_vm_config->err));
}



__API_FUNC_DECL(vm_get_instruction_count){
	return sll_current_instruction_count;
}



__API_FUNC_DECL(vm_get_instruction_index){
	return sll_current_instruction_index;
}



__API_FUNC_DECL(vm_get_location){
	sll_instruction_index_t ii;
	if (a<0){
		const sll_call_stack_t* c_st=sll_get_call_stack(sll_current_thread_index);
		ii=(c_st->l?(c_st->dt+c_st->l-1)->_ii:sll_current_instruction_index);
	}
	else{
		ii=(a>SLL_MAX_INSTRUCTION_INDEX?SLL_MAX_INSTRUCTION_INDEX:(sll_instruction_index_t)a);
	}
	return sll_instruction_to_location(ii);
}



__API_FUNC_DECL(vm_get_ref_count){
	return a->rc;
}

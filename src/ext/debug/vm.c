#include <debug/util.h>
#include <sll.h>



sll_object_t* debug_get_instruction_count(sll_object_t*const* al,sll_arg_count_t all){
	return SLL_FROM_INT(sll_current_instruction_count);
}



sll_object_t* debug_get_instruction_index(sll_object_t*const* al,sll_arg_count_t all){
	return SLL_FROM_INT(sll_current_instruction_index);
}



sll_object_t* debug_get_vm_config(sll_object_t*const* al,sll_arg_count_t all){
	sll_object_t* o=SLL_CREATE();
	o->t=SLL_OBJECT_TYPE_ARRAY;
	sll_array_create(5,&(o->dt.a));
	o->dt.a.v[0]=SLL_FROM_INT(sll_current_vm_config->s_sz);
	o->dt.a.v[1]=SLL_FROM_INT(sll_current_vm_config->c_st_sz);
	o->dt.a.v[2]=SLL_FROM_INT(sll_file_to_handle(sll_current_vm_config->in));
	o->dt.a.v[3]=SLL_FROM_INT(sll_file_to_handle(sll_current_vm_config->out));
	o->dt.a.v[4]=SLL_FROM_INT(sll_file_to_handle(sll_current_vm_config->err));
	return o;
}

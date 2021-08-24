#include <sll/common.h>
#include <sll/types.h>



__SLL_FUNC void sll_set_assembly_data_stack(sll_assembly_data_t* a_dt,sll_buffer_t bf,sll_stack_offset_t sz){
	a_dt->_s.ptr=bf;
	a_dt->_s.off=0;
	a_dt->_s.sz=sz;
}



__SLL_FUNC void sll_set_compilation_data_stack(sll_compilation_data_t* c_dt,sll_buffer_t bf,sll_stack_offset_t sz){
	c_dt->_s.ptr=bf;
	c_dt->_s.off=0;
	c_dt->_s.sz=sz;
}



__SLL_FUNC void sll_setup_stack(sll_stack_data_t* o,sll_buffer_t bf,sll_stack_offset_t sz){
	o->ptr=bf;
	o->off=0;
	o->sz=sz;
}

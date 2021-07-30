#include <lll_lib.h>
#include <_lll_lib_internal.h>
#include <stdint.h>



__LLL_IMPORT_EXPORT void lll_set_assembly_data_stack(lll_assembly_data_t* a_dt,uint8_t* bf,lll_stack_offset_t sz){
	a_dt->_s.ptr=bf;
	a_dt->_s.off=0;
	a_dt->_s.sz=sz;
}



__LLL_IMPORT_EXPORT void lll_set_compilation_data_stack(lll_compilation_data_t* c_dt,uint8_t* bf,lll_stack_offset_t sz){
	c_dt->_s.ptr=bf;
	c_dt->_s.off=0;
	c_dt->_s.sz=sz;
}



__LLL_IMPORT_EXPORT void lll_setup_stack(lll_stack_data_t* o,uint8_t* bf,lll_stack_offset_t sz){
	o->ptr=bf;
	o->off=0;
	o->sz=sz;
}

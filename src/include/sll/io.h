#ifndef __SLL_IO_H__
#define __SLL_IO_H__ 1
#include <sll/common.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_return_t sll_load_assembly(sll_file_t* rf,sll_assembly_data_t* a_dt,sll_error_t* e);



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_return_t sll_load_compiled_object(sll_file_t* rf,sll_compilation_data_t* c_dt,sll_error_t* e);



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_return_t sll_load_object(sll_compilation_data_t* c_dt,sll_file_t* rf,sll_object_t** o,sll_error_t* e);



__SLL_EXTERNAL void sll_write_assembly(sll_file_t* wf,const sll_assembly_data_t* o);



__SLL_EXTERNAL void sll_write_compiled_object(sll_file_t* wf,const sll_compilation_data_t* c_dt);



__SLL_EXTERNAL void sll_write_object(sll_file_t* wf,const sll_object_t* o);



#endif

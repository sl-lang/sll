#ifndef __SLL_ASSEMBLY_H__
#define __SLL_ASSEMBLY_H__ 1
#include <sll/common.h>
#include <sll/types.h>



extern const sll_runtime_data_t* sll_current_runtime_data;



__SLL_FUNC __SLL_CHECK_OUTPUT sll_return_code_t sll_execute_assembly(const sll_assembly_data_t* a_dt,const sll_stack_data_t* st,const sll_runtime_data_t* r_dt,sll_error_t* e);



__SLL_FUNC void sll_free_assembly_data(sll_assembly_data_t* a_dt);



__SLL_FUNC void sll_free_assembly_function_table(sll_assembly_function_table_t* ft);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_return_t sll_generate_assembly(const sll_compilation_data_t* c_dt,sll_assembly_data_t* o,sll_error_t* e);



__SLL_FUNC void sll_print_assembly(const sll_assembly_data_t* a_dt,sll_output_data_stream_t* os);



__SLL_FUNC void sll_set_assembly_data_stack(sll_assembly_data_t* a_dt,sll_buffer_t bf,sll_stack_offset_t sz);



#endif

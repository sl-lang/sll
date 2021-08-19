#ifndef __SLL_ASSEMBLY_H__
#define __SLL_ASSEMBLY_H__ 1
#include <sll/common.h>
#include <sll/types.h>



__SLL_FUNC __SLL_RETURN_CODE sll_execute_assembly(const sll_assembly_data_t* a_dt,const sll_stack_data_t* st,const sll_internal_function_table_t* i_ft,sll_input_data_stream_t* in,sll_output_data_stream_t* out,sll_error_t* e);



__SLL_FUNC void sll_free_assembly_data(sll_assembly_data_t* a_dt);



__SLL_FUNC void sll_free_assembly_function_table(sll_assembly_function_table_t* ft);



__SLL_FUNC __SLL_RETURN sll_generate_assembly(const sll_compilation_data_t* c_dt,sll_assembly_data_t* o,sll_error_t* e);



__SLL_FUNC void sll_print_assembly(const sll_assembly_data_t* a_dt,sll_output_data_stream_t* os);



__SLL_FUNC void sll_set_assembly_data_stack(sll_assembly_data_t* a_dt,sll_buffer_t bf,sll_stack_offset_t sz);



#endif

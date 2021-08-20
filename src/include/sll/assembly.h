#ifndef __SLL_ASSEMBLY_H__
#define __SLL_ASSEMBLY_H__ 1
#include <sll/common.h>
#include <sll/types.h>



__SLL_FUNC __SLL_RETURN_CODE sll_execute_assembly(const sll_assembly_data_t* restrict a_dt,const sll_stack_data_t* restrict st,const sll_internal_function_table_t* restrict i_ft,sll_input_data_stream_t* restrict in,sll_output_data_stream_t* restrict out,sll_error_t* restrict e);



__SLL_FUNC void sll_free_assembly_data(sll_assembly_data_t* restrict a_dt);



__SLL_FUNC void sll_free_assembly_function_table(sll_assembly_function_table_t* restrict ft);



__SLL_FUNC __SLL_RETURN sll_generate_assembly(const sll_compilation_data_t* restrict c_dt,sll_assembly_data_t* restrict o,sll_error_t* restrict e);



__SLL_FUNC void sll_print_assembly(const sll_assembly_data_t* restrict a_dt,sll_output_data_stream_t* restrict os);



__SLL_FUNC void sll_set_assembly_data_stack(sll_assembly_data_t* restrict a_dt,sll_buffer_t restrict bf,sll_stack_offset_t sz);



#endif

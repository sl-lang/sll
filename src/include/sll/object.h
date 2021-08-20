#ifndef __SLL_OBJECT_H__
#define __SLL_OBJECT_H__ 1
#include <sll/common.h>
#include <sll/types.h>



__SLL_FUNC __SLL_RETURN_SIZE sll_get_object_size(const sll_object_t* restrict o);



__SLL_FUNC __SLL_RETURN sll_insert_debug_object(sll_compilation_data_t* restrict c_dt,sll_input_data_stream_t* restrict is,sll_error_t* restrict e);



__SLL_FUNC void sll_optimize_object(sll_compilation_data_t* restrict c_dt,sll_object_t* restrict o);



__SLL_FUNC void sll_optimize_metadata(sll_compilation_data_t* restrict c_dt);



__SLL_FUNC __SLL_RETURN sll_parse_all_objects(sll_compilation_data_t* restrict c_dt,sll_internal_function_table_t* restrict i_ft,sll_import_loader_t il,sll_error_t* restrict e);



__SLL_FUNC __SLL_RETURN sll_parse_object(sll_compilation_data_t* restrict c_dt,sll_internal_function_table_t* restrict i_ft,sll_import_loader_t il,sll_error_t* restrict e,sll_object_t** restrict o);



__SLL_FUNC void sll_print_object(const sll_compilation_data_t* restrict c_dt,const sll_object_t* restrict o,sll_output_data_stream_t* restrict os);



__SLL_FUNC void sll_remove_object_debug_data(sll_object_t* restrict o);



__SLL_FUNC void sll_remove_object_padding(sll_compilation_data_t* restrict c_dt,sll_object_t* restrict o);



#endif

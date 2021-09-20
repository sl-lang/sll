#ifndef __SLL_OBJECT_H__
#define __SLL_OBJECT_H__ 1
#include <sll/common.h>
#include <sll/types.h>



__SLL_FUNC void sll_insert_debug_object(sll_compilation_data_t* c_dt,sll_input_data_stream_t* is);



__SLL_FUNC void sll_optimize_object(sll_compilation_data_t* c_dt,sll_internal_function_table_t* i_ft);



__SLL_FUNC void sll_optimize_metadata(sll_compilation_data_t* c_dt);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_return_t sll_parse_all_objects(sll_compilation_data_t* c_dt,sll_internal_function_table_t* i_ft,sll_import_loader_t il,sll_error_t* e);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_return_t sll_parse_object(sll_compilation_data_t* c_dt,sll_internal_function_table_t* i_ft,sll_import_loader_t il,sll_error_t* e,sll_object_t** o);



__SLL_FUNC void sll_print_object(const sll_compilation_data_t* c_dt,const sll_object_t* o,sll_output_data_stream_t* os);



__SLL_FUNC void sll_remove_object_debug_data(sll_object_t* o);



__SLL_FUNC void sll_remove_object_padding(sll_compilation_data_t* c_dt,sll_object_t* o);



__SLL_FUNC sll_object_t* sll_skip_object(sll_object_t* o);



__SLL_FUNC const sll_object_t* sll_skip_object_const(const sll_object_t* o);



#endif

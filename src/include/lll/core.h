#ifndef __LLL_CORE_H__
#define __LLL_CORE_H__ 1
#include <lll/common.h>
#include <lll/types.h>



__LLL_FUNC __LLL_RETURN_COMPARE lll_compare_runtime_object(const lll_runtime_object_t* a,const lll_runtime_object_t* b);



__LLL_FUNC void lll_create_internal_function_table(lll_internal_function_table_t* o);



__LLL_FUNC __LLL_RETURN_STRING_INDEX lll_create_string(lll_string_table_t* st,const lll_char_t* dt,lll_string_length_t l);



__LLL_FUNC __LLL_RETURN_CODE lll_execute_assembly(const lll_assembly_data_t* a_dt,const lll_stack_data_t* st,lll_internal_function_table_t* i_ft,lll_input_data_stream_t* in,lll_output_data_stream_t* out,lll_error_t* e);



__LLL_FUNC void lll_free_assembly_data(lll_assembly_data_t* a_dt);



__LLL_FUNC void lll_free_assembly_function_table(lll_assembly_function_table_t* ft);



__LLL_FUNC void lll_free_compilation_data(lll_compilation_data_t* c_dt);



__LLL_FUNC void lll_free_export_table(lll_export_table_t* et);



__LLL_FUNC void lll_free_function_table(lll_function_table_t* ft);



__LLL_FUNC void lll_free_identifier_table(lll_identifier_table_t* idt);



__LLL_FUNC void lll_free_internal_function_table(lll_internal_function_table_t* ift);



__LLL_FUNC void lll_free_string_table(lll_string_table_t* st);



__LLL_FUNC __LLL_RETURN lll_generate_assembly(const lll_compilation_data_t* c_dt,lll_assembly_data_t* o,lll_error_t* e);



__LLL_FUNC __LLL_RETURN_SIZE lll_get_object_size(const lll_object_t* o);



__LLL_FUNC void lll_init_compilation_data(const char* fp,lll_input_data_stream_t* is,lll_compilation_data_t* o);



__LLL_FUNC __LLL_RETURN lll_insert_debug_object(lll_compilation_data_t* c_dt,lll_input_data_stream_t* is,lll_error_t* e);



__LLL_FUNC __LLL_RETURN_FUNCTION_INDEX lll_lookup_internal_function(const lll_internal_function_table_t* i_ft,const char* nm);



__LLL_FUNC void lll_optimize_object(lll_compilation_data_t* c_dt,lll_object_t* o);



__LLL_FUNC void lll_optimize_metadata(lll_compilation_data_t* c_dt);



__LLL_FUNC __LLL_RETURN lll_parse_all_objects(lll_compilation_data_t* c_dt,lll_internal_function_table_t* i_ft,lll_import_loader_t il,lll_error_t* e);



__LLL_FUNC __LLL_RETURN lll_parse_object(lll_compilation_data_t* c_dt,lll_internal_function_table_t* i_ft,lll_import_loader_t il,lll_error_t* e,lll_object_t** o);



__LLL_FUNC void lll_print_assembly(const lll_assembly_data_t* a_dt,lll_output_data_stream_t* os);



__LLL_FUNC void lll_print_error(lll_input_data_stream_t* is,const lll_error_t* e);



__LLL_FUNC void lll_print_object(const lll_compilation_data_t* c_dt,const lll_object_t* o,lll_output_data_stream_t* os);



__LLL_FUNC lll_function_index_t lll_register_internal_function(lll_internal_function_table_t* i_ft,const char* nm,lll_internal_function_pointer_t f);



__LLL_FUNC void lll_register_standard_internal_functions(lll_internal_function_table_t* i_ft);



__LLL_FUNC void lll_remove_object_debug_data(lll_object_t* o);



__LLL_FUNC void lll_remove_object_padding(lll_compilation_data_t* c_dt,lll_object_t* o);



__LLL_FUNC __LLL_RETURN_COMPARE lll_runtime_object_nonzero(const lll_runtime_object_t* o);



__LLL_FUNC void lll_set_argument_count(lll_sys_arg_count_t ac);



__LLL_FUNC void lll_set_argument(lll_sys_arg_count_t i,const char* a);



__LLL_FUNC void lll_set_assembly_data_stack(lll_assembly_data_t* a_dt,lll_buffer_t bf,lll_stack_offset_t sz);



__LLL_FUNC void lll_set_compilation_data_stack(lll_compilation_data_t* c_dt,lll_buffer_t bf,lll_stack_offset_t sz);



__LLL_FUNC void lll_setup_stack(lll_stack_data_t* o,lll_buffer_t bf,lll_stack_offset_t sz);



#endif

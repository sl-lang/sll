#ifndef __SLL_CORE_H__
#define __SLL_CORE_H__ 1
#include <sll/common.h>
#include <sll/types.h>



__SLL_FUNC void sll_create_internal_function_table(sll_internal_function_table_t* o);



__SLL_FUNC __SLL_RETURN_STRING_INDEX sll_create_string(sll_string_table_t* st,const sll_char_t* dt,sll_string_length_t l);



__SLL_FUNC __SLL_RETURN_CODE sll_execute_assembly(const sll_assembly_data_t* a_dt,const sll_stack_data_t* st,const sll_internal_function_table_t* i_ft,sll_input_data_stream_t* in,sll_output_data_stream_t* out,sll_error_t* e);



__SLL_FUNC void sll_free_assembly_data(sll_assembly_data_t* a_dt);



__SLL_FUNC void sll_free_assembly_function_table(sll_assembly_function_table_t* ft);



__SLL_FUNC void sll_free_compilation_data(sll_compilation_data_t* c_dt);



__SLL_FUNC void sll_free_export_table(sll_export_table_t* et);



__SLL_FUNC void sll_free_function_table(sll_function_table_t* ft);



__SLL_FUNC void sll_free_identifier_table(sll_identifier_table_t* idt);



__SLL_FUNC void sll_free_internal_function_table(sll_internal_function_table_t* ift);



__SLL_FUNC void sll_free_string_table(sll_string_table_t* st);



__SLL_FUNC __SLL_RETURN sll_generate_assembly(const sll_compilation_data_t* c_dt,sll_assembly_data_t* o,sll_error_t* e);



__SLL_FUNC __SLL_RETURN_SIZE sll_get_object_size(const sll_object_t* o);



__SLL_FUNC void sll_init_compilation_data(const sll_char_t* fp,sll_input_data_stream_t* is,sll_compilation_data_t* o);



__SLL_FUNC __SLL_RETURN sll_insert_debug_object(sll_compilation_data_t* c_dt,sll_input_data_stream_t* is,sll_error_t* e);



__SLL_FUNC __SLL_RETURN_FUNCTION_INDEX sll_lookup_internal_function(const sll_internal_function_table_t* i_ft,const char* nm);



__SLL_FUNC sll_string_t* sll_object_to_string(sll_runtime_object_t* a,sll_array_length_t al);



__SLL_FUNC sll_string_length_t sll_object_to_string_length(sll_runtime_object_t* a,sll_array_length_t al);



__SLL_FUNC void sll_optimize_object(sll_compilation_data_t* c_dt,sll_object_t* o);



__SLL_FUNC void sll_optimize_metadata(sll_compilation_data_t* c_dt);



__SLL_FUNC __SLL_RETURN sll_parse_all_objects(sll_compilation_data_t* c_dt,sll_internal_function_table_t* i_ft,sll_import_loader_t il,sll_error_t* e);



__SLL_FUNC __SLL_RETURN sll_parse_object(sll_compilation_data_t* c_dt,sll_internal_function_table_t* i_ft,sll_import_loader_t il,sll_error_t* e,sll_object_t** o);



__SLL_FUNC void sll_print_assembly(const sll_assembly_data_t* a_dt,sll_output_data_stream_t* os);



__SLL_FUNC void sll_print_error(sll_input_data_stream_t* is,const sll_error_t* e);



__SLL_FUNC void sll_print_object(const sll_compilation_data_t* c_dt,const sll_object_t* o,sll_output_data_stream_t* os);



__SLL_FUNC sll_function_index_t sll_register_internal_function(sll_internal_function_table_t* i_ft,const char* nm,sll_internal_function_pointer_t f);



__SLL_FUNC void sll_register_standard_internal_functions(sll_internal_function_table_t* i_ft);



__SLL_FUNC void sll_remove_object_debug_data(sll_object_t* o);



__SLL_FUNC void sll_remove_object_padding(sll_compilation_data_t* c_dt,sll_object_t* o);



__SLL_FUNC void sll_set_argument_count(sll_sys_arg_count_t ac);



__SLL_FUNC void sll_set_argument(sll_sys_arg_count_t i,const char* a);



__SLL_FUNC void sll_set_assembly_data_stack(sll_assembly_data_t* a_dt,sll_buffer_t bf,sll_stack_offset_t sz);



__SLL_FUNC void sll_set_compilation_data_stack(sll_compilation_data_t* c_dt,sll_buffer_t bf,sll_stack_offset_t sz);



__SLL_FUNC void sll_setup_stack(sll_stack_data_t* o,sll_buffer_t bf,sll_stack_offset_t sz);



#endif

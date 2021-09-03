#ifndef __SLL_CORE_H__
#define __SLL_CORE_H__ 1
#include <sll/common.h>
#include <sll/types.h>



__SLL_FUNC __SLL_RETURN_STRING_INDEX sll_add_string(sll_string_table_t* st,sll_string_t* s);



__SLL_FUNC __SLL_RETURN_STRING_INDEX sll_create_string(sll_string_table_t* st,const sll_char_t* dt,sll_string_length_t l);



__SLL_FUNC void sll_free_compilation_data(sll_compilation_data_t* c_dt);



__SLL_FUNC void sll_free_export_table(sll_export_table_t* et);



__SLL_FUNC void sll_free_function_table(sll_function_table_t* ft);



__SLL_FUNC void sll_free_identifier_table(sll_identifier_table_t* idt);



__SLL_FUNC void sll_free_runtime_object_stack_data(sll_runtime_object_stack_data_t* rst);



__SLL_FUNC void sll_free_string_table(sll_string_table_t* st);



__SLL_FUNC void sll_init_compilation_data(const sll_char_t* fp,sll_input_data_stream_t* is,sll_compilation_data_t* o);



__SLL_FUNC void sll_print_error(sll_input_data_stream_t* is,const sll_error_t* e);



__SLL_FUNC void sll_register_cleanup(sll_cleanup_function f,sll_cleanup_type_t t);



__SLL_FUNC void sll_set_compilation_data_stack(sll_compilation_data_t* c_dt,sll_buffer_t bf,sll_stack_offset_t sz);



__SLL_FUNC void sll_setup_stack(sll_stack_data_t* o,sll_buffer_t bf,sll_stack_offset_t sz);



#endif

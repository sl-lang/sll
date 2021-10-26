#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/identifier.h>
#include <sll/init.h>
#include <sll/platform.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/util.h>



__SLL_FUNC void sll_init(void){
	sll_platform_setup_console();
	atexit(sll_deinit);
}



__SLL_FUNC void sll_init_array(sll_array_t* o){
	SLL_INIT_ARRAY(o);
}



__SLL_FUNC void sll_init_assembly_data(sll_assembly_data_t* o){
	SLL_INIT_ASSEMBLY_DATA(o);
}



__SLL_FUNC void sll_init_assembly_function_table(sll_assembly_function_table_t* o){
	SLL_INIT_ASSEMBLY_FUNCTION_TABLE(o);
}



__SLL_FUNC void sll_init_assembly_instruction_data(sll_assembly_instruction_data_t* o){
	SLL_INIT_ASSEMBLY_INSTRUCTION_DATA(o);
}



__SLL_FUNC void sll_init_assembly_instruction(sll_assembly_instruction_t* o){
	SLL_INIT_ASSEMBLY_INSTRUCTION(o);
}



__SLL_FUNC void sll_init_assembly_stack_data(sll_assembly_stack_data_t* o){
	SLL_INIT_ASSEMBLY_STACK_DATA(o);
}



__SLL_FUNC void sll_init_compilation_data(const sll_char_t* fp,sll_input_data_stream_t* is,sll_compilation_data_t* o){
	o->is=is;
	o->tm=sll_platform_get_current_time();
	o->h=NULL;
	SLL_INIT_IDENTIFIER_TABLE(&(o->idt));
	SLL_INIT_EXPORT_TABLE(&(o->et));
	SLL_INIT_FUNCTION_TABLE(&(o->ft));
	SLL_INIT_STRING_TABLE(&(o->st));
	_init_object_stack(o);
	o->_n_sc_id=1;
	IGNORE_RESULT(sll_create_string(&(o->st),fp,sll_string_length_unaligned(fp)));
}



__SLL_FUNC void sll_init_compilation_stack_data(sll_compilation_stack_data_t* o){
	SLL_INIT_COMPILATION_STACK_DATA(o);
}



__SLL_FUNC void sll_init_debug_object_data(sll_debug_object_data_t* o){
	SLL_INIT_DEBUG_OBJECT_DATA(o);
}



__SLL_FUNC void sll_init_error_data_range(sll_error_data_range_t* o){
	SLL_INIT_ERROR_DATA_RANGE(o);
}



__SLL_FUNC void sll_init_error_data(sll_error_data_t* o){
	SLL_INIT_ERROR_DATA(o);
}



__SLL_FUNC void sll_init_error(sll_error_t* o){
	SLL_INIT_ERROR(o);
}



__SLL_FUNC void sll_init_export_table(sll_export_table_t* o){
	SLL_INIT_EXPORT_TABLE(o);
}



__SLL_FUNC void sll_init_function_object_data(sll_function_object_data_t* o){
	SLL_INIT_FUNCTION_OBJECT_DATA(o);
}



__SLL_FUNC void sll_init_function(sll_function_t* o){
	SLL_INIT_FUNCTION(o);
}



__SLL_FUNC void sll_init_function_table(sll_function_table_t* o){
	SLL_INIT_FUNCTION_TABLE(o);
}



__SLL_FUNC void sll_init_handle_data(sll_handle_data_t* o){
	SLL_INIT_HANDLE_DATA(o);
}



__SLL_FUNC void sll_init_handle_descriptor(sll_handle_descriptor_t* o){
	SLL_INIT_HANDLE_DESCRIPTOR(o);
}



__SLL_FUNC void sll_init_handle_list(sll_handle_list_t* o){
	SLL_INIT_HANDLE_LIST(o);
}



__SLL_FUNC void sll_init_header(sll_header_t* o){
	SLL_INIT_HEADER(o);
}



__SLL_FUNC void sll_init_header_list(sll_header_list_t* o){
	SLL_INIT_HEADER_LIST(o);
}



__SLL_FUNC void sll_init_http_response(sll_http_response_t* o){
	SLL_INIT_HTTP_RESPONSE(o);
}



__SLL_FUNC void sll_init_identifier_list(sll_identifier_list_t* o){
	SLL_INIT_IDENTIFIER_LIST(o);
}



__SLL_FUNC void sll_init_identifier(sll_identifier_t* o){
	SLL_INIT_IDENTIFIER(o);
}



__SLL_FUNC void sll_init_identifier_table(sll_identifier_table_t* o){
	SLL_INIT_IDENTIFIER_TABLE(o);
}



__SLL_FUNC void sll_init_input_buffer(sll_input_buffer_t* o){
	SLL_INIT_INPUT_BUFFER(o);
}



__SLL_FUNC void sll_init_input_data_stream(sll_input_data_stream_t* o){
	SLL_INIT_INPUT_DATA_STREAM(o);
}



__SLL_FUNC void sll_init_integer_heap_queue(sll_integer_heap_queue_t* o){
	SLL_INIT_INTEGER_HEAP_QUEUE(o);
}



__SLL_FUNC void sll_init_internal_function(sll_internal_function_t* o){
	SLL_INIT_INTERNAL_FUNCTION(o);
}



__SLL_FUNC void sll_init_internal_function_table(sll_internal_function_table_t* o){
	SLL_INIT_INTERNAL_FUNCTION_TABLE(o);
}



__SLL_FUNC void sll_init_json_array(sll_json_array_t* o){
	SLL_INIT_JSON_ARRAY(o);
}



__SLL_FUNC void sll_init_json_map_keypair(sll_json_map_keypair_t* o){
	SLL_INIT_JSON_MAP_KEYPAIR(o);
}



__SLL_FUNC void sll_init_json_map(sll_json_map_t* o){
	SLL_INIT_JSON_MAP(o);
}



__SLL_FUNC void sll_init_json_object_data(sll_json_object_data_t* o){
	SLL_INIT_JSON_OBJECT_DATA(o);
}



__SLL_FUNC void sll_init_json_object(sll_json_object_t* o){
	SLL_INIT_JSON_OBJECT(o);
}



__SLL_FUNC void sll_init_loop_data(sll_loop_data_t* o){
	SLL_INIT_LOOP_DATA(o);
}



__SLL_FUNC void sll_init_map(sll_map_t* o){
	SLL_INIT_MAP(o);
}



__SLL_FUNC void sll_init_object_data(sll_object_data_t* o){
	SLL_INIT_OBJECT_DATA(o);
}



__SLL_FUNC void sll_init_object(sll_object_t* o){
	SLL_INIT_OBJECT(o);
}



__SLL_FUNC void sll_init_output_data_stream(sll_output_data_stream_t* o){
	SLL_INIT_OUTPUT_DATA_STREAM(o);
}



__SLL_FUNC void sll_init_runtime_data(sll_runtime_data_t* o){
	SLL_INIT_RUNTIME_DATA(o);
}



__SLL_FUNC void sll_init_runtime_object_data(sll_runtime_object_data_t* o){
	SLL_INIT_RUNTIME_OBJECT_DATA(o);
}



__SLL_FUNC void sll_init_runtime_object(sll_runtime_object_t* o){
	SLL_INIT_RUNTIME_OBJECT(o);
}



__SLL_FUNC void sll_init_stack_data(sll_stack_data_t* o){
	SLL_INIT_STACK_DATA(o);
}



__SLL_FUNC void sll_init_string(sll_string_t* o){
	SLL_INIT_STRING(o);
}



__SLL_FUNC void sll_init_string_table(sll_string_table_t* o){
	SLL_INIT_STRING_TABLE(o);
}

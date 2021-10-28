#ifndef __SLL_INIT_H__
#define __SLL_INIT_H__ 1
#include <sll/api/json.h>
#include <sll/assembly.h>
#include <sll/common.h>
#include <sll/error.h>
#include <sll/gc.h>
#include <sll/handle.h>
#include <sll/identifier.h>
#include <sll/object.h>
#include <sll/runtime_object.h>
#include <sll/types.h>



#define SLL_INIT_ARRAY(o) \
	do{ \
		(o)->l=0; \
		(o)->v=NULL; \
	} while (0)
#define SLL_INIT_ASSEMBLY_DATA(o) \
	do{ \
		(o)->tm=0; \
		(o)->h=NULL; \
		(o)->ic=0; \
		(o)->vc=0; \
		SLL_INIT_ASSEMBLY_FUNCTION_TABLE(&((o)->ft)); \
		SLL_INIT_STRING_TABLE(&((o)->st)); \
		SLL_INIT_ASSEMBLY_STACK_DATA(&((o)->_s)); \
	} while (0)
#define SLL_INIT_ASSEMBLY_FUNCTION_TABLE(o) \
	do{ \
		(o)->l=0; \
		(o)->dt=NULL; \
	} while (0)
#define SLL_INIT_ASSEMBLY_INSTRUCTION(o) \
	do{ \
		(o)->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP; \
	} while (0)
#define SLL_INIT_ASSEMBLY_INSTRUCTION_DATA(o) \
	do{ \
		(o)->i=0; \
	} while (0)
#define SLL_INIT_ASSEMBLY_STACK_DATA(o) \
	do{ \
		(o)->s=NULL; \
		(o)->e=NULL; \
		(o)->c=0; \
		(o)->p=NULL; \
	} while (0)
#define SLL_INIT_COMPILATION_DATA(o) \
	do{ \
		SLL_INIT_INPUT_DATA_STREAM((o)->is); \
		(o)->tm=0; \
		(o)->h=NULL; \
		SLL_INIT_IDENTIFIER_TABLE(&((o)->idt)); \
		SLL_INIT_EXPORT_TABLE(&((o)->et)); \
		SLL_INIT_FUNCTION_TABLE(&((o)->ft)); \
		SLL_INIT_STRING_TABLE(&((o)->st)); \
		SLL_INIT_COMPILATION_STACK_DATA(&((o)->_s));\
		(o)->_n_sc_id=0; \
	} while (0)
#define SLL_INIT_COMPILATION_STACK_DATA(o) \
	do{ \
		(o)->s=NULL; \
		(o)->e=NULL; \
		(o)->c=0; \
		(o)->p=NULL; \
		(o)->off=0; \
	} while (0)
#define SLL_INIT_DEBUG_OBJECT_DATA(o) \
	do{ \
		(o)->fpi=0; \
		(o)->ln=0; \
		(o)->cn=0; \
		(o)->ln_off=0; \
	} while (0)
#define SLL_INIT_ERROR(o) \
	do{ \
		(o)->t=SLL_ERROR_UNKNOWN; \
	} while (0)
#define SLL_INIT_ERROR_DATA(o) \
	do{ \
		(o)->str[0]=0; \
	} while (0)
#define SLL_INIT_ERROR_DATA_RANGE(o) \
	do{ \
		(o)->off=0; \
		(o)->sz=0; \
	} while (0)
#define SLL_INIT_EXPORT_TABLE(o) \
	do{ \
		(o)->dt=NULL; \
		(o)->l=0; \
	} while (0)
#define SLL_INIT_FUNCTION(o) \
	do{ \
		(o)->off=0; \
		(o)->al=0; \
	} while (0)
#define SLL_INIT_FUNCTION_OBJECT_DATA(o) \
	do{ \
		(o)->ac=0; \
		(o)->id=0; \
		(o)->sc=0; \
	} while (0)
#define SLL_INIT_FUNCTION_TABLE(o) \
	do{ \
		(o)->dt=NULL; \
		(o)->l=0; \
	} while (0)
#define SLL_INIT_HANDLE_DATA(o) \
	do{ \
		(o)->t=SLL_HANDLE_UNKNOWN_TYPE; \
		(o)->h=0; \
	} while (0)
#define SLL_INIT_HANDLE_DESCRIPTOR(o) \
	do{ \
		(o)->nm[0]=0; \
		(o)->nml=0; \
		(o)->c=0; \
		(o)->sf=NULL; \
		(o)->df=NULL; \
	} while (0)
#define SLL_INIT_HANDLE_LIST(o) \
	do{ \
		(o)->dt=NULL; \
		(o)->dtl=0; \
	} while (0)
#define SLL_INIT_HEADER(o) \
	do{ \
		SLL_INIT_STRING(&((o)->k)); \
		SLL_INIT_STRING(&((o)->v)); \
	} while (0)
#define SLL_INIT_HEADER_LIST(o) \
	do{ \
		(o)->dt=NULL; \
		(o)->l=0; \
	} while (0)
#define SLL_INIT_HTTP_RESPONSE(o) \
	do{ \
		(o)->rc=NULL; \
		(o)->hl=NULL; \
		(o)->dt=NULL; \
	} while (0)
#define SLL_INIT_IDENTIFIER(o) \
	do{ \
		(o)->sc=0; \
		(o)->i=0; \
	} while (0)
#define SLL_INIT_IDENTIFIER_LIST(o) \
	do{ \
		(o)->dt=NULL; \
		(o)->l=0; \
	} while (0)
#define SLL_INIT_IDENTIFIER_TABLE(o) \
	do{ \
		for (unsigned int __i=0;__i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;__i++){ \
			SLL_INIT_IDENTIFIER_LIST((o)->s+__i); \
		} \
		(o)->il=NULL; \
		(o)->ill=0; \
	} while (0)
#define SLL_INIT_INPUT_BUFFER(o) \
	do{ \
		(o)->bf=NULL; \
		(o)->sz=0; \
	} while (0)
#define SLL_INIT_INPUT_DATA_STREAM(o) \
	do{ \
		(o)->ctx=NULL; \
		(o)->rf=NULL; \
		(o)->rbf=NULL; \
		(o)->rlf=NULL; \
		(o)->_lc=0; \
		(o)->_off=0; \
		(o)->_loff=0; \
	} while (0)
#define SLL_INIT_binary_heap(o) \
	do{ \
		(o)->v=NULL; \
		(o)->l=0; \
	} while (0)
#define SLL_INIT_INTERNAL_FUNCTION(o) \
	do{ \
		(o)->nm[0]=0; \
		(o)->nml=0; \
		(o)->c=0; \
		(o)->p=NULL; \
		(o)->t=0; \
	} while (0)
#define SLL_INIT_INTERNAL_FUNCTION_TABLE(o) \
	do{ \
		(o)->dt=NULL; \
		(o)->l=0; \
	} while (0)
#define SLL_INIT_JSON_ARRAY(o) \
	do{ \
		(o)->l=0; \
		(o)->dt=NULL; \
	} while (0)
#define SLL_INIT_JSON_MAP(o) \
	do{ \
		(o)->l=0; \
		(o)->dt=NULL; \
	} while (0)
#define SLL_INIT_JSON_MAP_KEYPAIR(o) \
	do{ \
		SLL_INIT_STRING(&((o)->k)); \
		SLL_INIT_JSON_OBJECT(&((o)->v)); \
	} while (0)
#define SLL_INIT_JSON_OBJECT(o) \
	do{ \
		(o)->t=SLL_JSON_OBJECT_TYPE_INTEGER; \
		(o)->dt.i=0; \
	} while (0)
#define SLL_INIT_JSON_OBJECT_DATA(o) \
	do{ \
		(o)->i=0; \
	} while (0)
#define SLL_INIT_LOOP_DATA(o) \
	do{ \
		(o)->ac=0; \
		(o)->sc=0; \
	} while (0)
#define SLL_INIT_MAP(o) \
	do{ \
		(o)->l=0; \
		(o)->v=NULL; \
	} while (0)
#define SLL_INIT_OBJECT(o) \
	do{ \
		(o)->t=SLL_OBJECT_TYPE_INT; \
		(o)->dt.i=0; \
	} while (0)
#define SLL_INIT_OBJECT_DATA(o) \
	do{ \
		(o)->i=0; \
	} while (0)
#define SLL_INIT_OUTPUT_DATA_STREAM(o) \
	do{ \
		(o)->ctx=NULL; \
		(o)->wcf=NULL; \
		(o)->wf=NULL; \
	} while (0)
#define SLL_INIT_RUNTIME_DATA(o) \
	do{ \
		(o)->ift=NULL; \
		(o)->hl=0; \
		(o)->is=NULL; \
		(o)->os=NULL; \
	} while (0)
#define SLL_INIT_RUNTIME_OBJECT(o) \
	do{ \
		(o)->rc=1; \
		(o)->t=SLL_RUNTIME_OBJECT_TYPE_INT; \
		SLL_GC_ZERO_DEBUG_DATA(o); \
		(o)->dt.i=0; \
	} while (0)
#define SLL_INIT_RUNTIME_OBJECT_DATA(o) \
	do{ \
		(o)->i=0; \
	} while (0)
#define SLL_INIT_STACK_DATA(o) \
	do{ \
		(o)->ptr=NULL; \
		(o)->off=0; \
		(o)->sz=0; \
	} while (0)
#define SLL_INIT_STRING(o) \
	do{ \
		(o)->l=0; \
		(o)->c=0; \
		(o)->v=NULL; \
	} while (0)
#define SLL_INIT_STRING_TABLE(o) \
	do{ \
		(o)->l=0; \
		(o)->dt=NULL; \
	} while (0)

#define SLL_INIT_ARRAY_STRUCT {0,NULL}
#define SLL_INIT_ASSEMBLY_DATA_STRUCT {0,NULL,0,0,SLL_INIT_ASSEMBLY_FUNCTION_TABLE_STRUCT,SLL_INIT_STRING_TABLE_STRUCT,SLL_INIT_ASSEMBLY_STACK_DATA_STRUCT}
#define SLL_INIT_ASSEMBLY_FUNCTION_TABLE_STRUCT {0,NULL}
#define SLL_INIT_ASSEMBLY_INSTRUCTION_DATA_UNION {.i=0}
#define SLL_INIT_ASSEMBLY_INSTRUCTION_STRUCT {SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP}
#define SLL_INIT_ASSEMBLY_STACK_DATA_STRUCT {NULL,NULL,0,NULL}
#define SLL_INIT_COMPILATION_DATA_STRUCT {NULL,0,NULL,SLL_INIT_IDENTIFIER_TABLE_STRUCT,SLL_INIT_EXPORT_TABLE_STRUCT,SLL_INIT_FUNCTION_TABLE_STRUCT,SLL_INIT_STRING_TABLE_STRUCT,SLL_INIT_COMPILATION_STACK_DATA_STRUCT,0}
#define SLL_INIT_COMPILATION_STACK_DATA_STRUCT {NULL,NULL,0,NULL,0}
#define SLL_INIT_DEBUG_OBJECT_DATA_STRUCT {0,0,0,0}
#define SLL_INIT_ERROR_DATA_RANGE_STRUCT {0,0}
#define SLL_INIT_ERROR_DATA_UNION {.str={0}}
#define SLL_INIT_ERROR_STRUCT {SLL_ERROR_UNKNOWN}
#define SLL_INIT_EXPORT_TABLE_STRUCT {NULL,0}
#define SLL_INIT_FUNCTION_OBJECT_DATA_STRUCT {0,0,0}
#define SLL_INIT_FUNCTION_STRUCT {0,0}
#define SLL_INIT_FUNCTION_TABLE_STRUCT {NULL,0}
#define SLL_INIT_HANDLE_DATA_STRUCT {SLL_HANDLE_UNKNOWN_TYPE,0}
#define SLL_INIT_HANDLE_DESCRIPTOR_STRUCT {{0},0,0,NULL,NULL}
#define SLL_INIT_HANDLE_LIST_STRUCT {NULL,0}
#define SLL_INIT_HEADER_LIST_STRUCT {NULL,0}
#define SLL_INIT_HEADER_STRUCT {SLL_INIT_STRING_STRUCT,SLL_INIT_STRING_STRUCT}
#define SLL_INIT_HTTP_RESPONSE_STRUCT {NULL,NULL,NULL}
#define SLL_INIT_IDENTIFIER_LIST_STRUCT {NULL,0}
#define SLL_INIT_IDENTIFIER_STRUCT {0,0}
#define SLL_INIT_IDENTIFIER_TABLE_STRUCT {.il=NULL,.ill=0}
#define SLL_INIT_INPUT_BUFFER_STRUCT {NULL,0}
#define SLL_INIT_INPUT_DATA_STREAM_STRUCT {NULL,NULL,NULL,NULL,0,0,0}
#define SLL_INIT_binary_heap_STRUCT {NULL,0}
#define SLL_INIT_INTERNAL_FUNCTION_STRUCT {{0},0,0,NULL,0}
#define SLL_INIT_INTERNAL_FUNCTION_TABLE_STRUCT {NULL,0}
#define SLL_INIT_JSON_ARRAY_STRUCT {0,NULL}
#define SLL_INIT_JSON_MAP_KEYPAIR_STRUCT {SLL_INIT_STRING_STRUCT,SLL_INIT_JSON_OBJECT_STRUCT}
#define SLL_INIT_JSON_MAP_STRUCT {0,NULL}
#define SLL_INIT_JSON_OBJECT_DATA_UNION {.i=0}
#define SLL_INIT_JSON_OBJECT_STRUCT {SLL_JSON_OBJECT_TYPE_INTEGER,{.i=0}}
#define SLL_INIT_LOOP_DATA_STRUCT {0,0}
#define SLL_INIT_MAP_STRUCT {0,NULL}
#define SLL_INIT_OBJECT_DATA_UNION {.i=0}
#define SLL_INIT_OBJECT_STRUCT {SLL_OBJECT_TYPE_INT,{.i=0}}
#define SLL_INIT_OUTPUT_DATA_STREAM_STRUCT {NULL,NULL,NULL}
#define SLL_INIT_RUNTIME_DATA_STRUCT {NULL,0,NULL,NULL}
#define SLL_INIT_RUNTIME_OBJECT_DATA_UNION {.i=0}
#define SLL_INIT_RUNTIME_OBJECT_STRUCT {1,SLL_RUNTIME_OBJECT_TYPE_INT,SLL_GC_ZERO_DEBUG_DATA_STRUCT,.dt=SLL_INIT_RUNTIME_OBJECT_DATA_UNION}
#define SLL_INIT_STACK_DATA_STRUCT {NULL,0,0}
#define SLL_INIT_STRING_STRUCT {0,0,NULL}
#define SLL_INIT_STRING_TABLE_STRUCT {0,NULL}




__SLL_FUNC void sll_deinit(void);



__SLL_FUNC void sll_deinit_array(sll_array_t* a);



__SLL_FUNC void sll_deinit_assembly_data(sll_assembly_data_t* a_dt);



__SLL_FUNC void sll_deinit_assembly_function_table(sll_assembly_function_table_t* ft);



__SLL_FUNC void sll_deinit_assembly_instruction(sll_assembly_instruction_t* ai);



__SLL_FUNC void sll_deinit_assembly_instruction_data(sll_assembly_instruction_data_t* ai_dt);



__SLL_FUNC void sll_deinit_assembly_stack_data(sll_assembly_stack_data_t* a_st);



__SLL_FUNC void sll_deinit_compilation_data(sll_compilation_data_t* c_dt);



__SLL_FUNC void sll_deinit_compilation_stack_data(sll_compilation_stack_data_t* c_st);



__SLL_FUNC void sll_deinit_debug_object_data(sll_debug_object_data_t* d_dt);



__SLL_FUNC void sll_deinit_error_data_range(sll_error_data_range_t* e_dt_r);



__SLL_FUNC void sll_deinit_error_data(sll_error_data_t* e_dt);



__SLL_FUNC void sll_deinit_error(sll_error_t* e);



__SLL_FUNC void sll_deinit_export_table(sll_export_table_t* et);



__SLL_FUNC void sll_deinit_function_object_data(sll_function_object_data_t* f_dt);



__SLL_FUNC void sll_deinit_function(sll_function_t* f);



__SLL_FUNC void sll_deinit_function_table(sll_function_table_t* ft);



__SLL_FUNC void sll_deinit_handle_data(sll_handle_data_t* h_dt);



__SLL_FUNC void sll_deinit_handle_descriptor(sll_handle_descriptor_t* hd);



__SLL_FUNC void sll_deinit_handle_list(sll_handle_list_t* hl);



__SLL_FUNC void sll_deinit_header(sll_header_t* h);



__SLL_FUNC void sll_deinit_header_list(sll_header_list_t* hl);



__SLL_FUNC void sll_deinit_http_response(sll_http_response_t* r);



__SLL_FUNC void sll_deinit_identifier(sll_identifier_t* i);



__SLL_FUNC void sll_deinit_identifier_list(sll_identifier_list_t* il);



__SLL_FUNC void sll_deinit_identifier_table(sll_identifier_table_t* idt);



__SLL_FUNC void sll_deinit_input_buffer(sll_input_buffer_t* i_bf);



__SLL_FUNC void sll_deinit_input_data_stream(sll_input_data_stream_t* is);



__SLL_FUNC void sll_deinit_binary_heap(sll_binary_heap_t* il);



__SLL_FUNC void sll_deinit_internal_function(sll_internal_function_t* i_f);



__SLL_FUNC void sll_deinit_internal_function_table(sll_internal_function_table_t* ift);



__SLL_FUNC void sll_deinit_json_array(sll_json_array_t* json_a);



__SLL_FUNC void sll_deinit_json_map_keypair(sll_json_map_keypair_t* json_kp);



__SLL_FUNC void sll_deinit_json_map(sll_json_map_t* json_m);



__SLL_FUNC void sll_deinit_json_object_data(sll_json_object_data_t* json_dt);



__SLL_FUNC void sll_deinit_json_object(sll_json_object_t* json);



__SLL_FUNC void sll_deinit_loop_data(sll_loop_data_t* l_dt);



__SLL_FUNC void sll_deinit_map(sll_map_t* m);



__SLL_FUNC void sll_deinit_object_data(sll_object_data_t* o_dt);



__SLL_FUNC void sll_deinit_object(sll_object_t* o);



__SLL_FUNC void sll_deinit_output_data_stream(sll_output_data_stream_t* os);



__SLL_FUNC void sll_deinit_runtime_data(sll_runtime_data_t* r_dt);



__SLL_FUNC void sll_deinit_runtime_object_data(sll_runtime_object_data_t* ro_dt);



__SLL_FUNC void sll_deinit_runtime_object(sll_runtime_object_t* o);



__SLL_FUNC void sll_deinit_stack_data(sll_stack_data_t* s_dt);



__SLL_FUNC void sll_deinit_string(sll_string_t* s);



__SLL_FUNC void sll_deinit_string_table(sll_string_table_t* st);



__SLL_FUNC void sll_init(void);



__SLL_FUNC void sll_init_array(sll_array_t* o);



__SLL_FUNC void sll_init_assembly_data(sll_assembly_data_t* o);



__SLL_FUNC void sll_init_assembly_function_table(sll_assembly_function_table_t* o);



__SLL_FUNC void sll_init_assembly_instruction(sll_assembly_instruction_t* o);



__SLL_FUNC void sll_init_assembly_instruction_data(sll_assembly_instruction_data_t* o);



__SLL_FUNC void sll_init_assembly_stack_data(sll_assembly_stack_data_t* o);



__SLL_FUNC void sll_init_compilation_data(const sll_char_t* fp,sll_input_data_stream_t* is,sll_compilation_data_t* o);



__SLL_FUNC void sll_init_compilation_stack_data(sll_compilation_stack_data_t* o);



__SLL_FUNC void sll_init_debug_object_data(sll_debug_object_data_t* o);



__SLL_FUNC void sll_init_error_data_range(sll_error_data_range_t* o);



__SLL_FUNC void sll_init_error_data(sll_error_data_t* o);



__SLL_FUNC void sll_init_error(sll_error_t* o);



__SLL_FUNC void sll_init_export_table(sll_export_table_t* o);



__SLL_FUNC void sll_init_function_object_data(sll_function_object_data_t* o);



__SLL_FUNC void sll_init_function(sll_function_t* o);



__SLL_FUNC void sll_init_function_table(sll_function_table_t* o);



__SLL_FUNC void sll_init_handle_data(sll_handle_data_t* o);



__SLL_FUNC void sll_init_handle_descriptor(sll_handle_descriptor_t* o);



__SLL_FUNC void sll_init_handle_list(sll_handle_list_t* o);



__SLL_FUNC void sll_init_header(sll_header_t* o);



__SLL_FUNC void sll_init_header_list(sll_header_list_t* o);



__SLL_FUNC void sll_init_http_response(sll_http_response_t* o);



__SLL_FUNC void sll_init_identifier(sll_identifier_t* o);



__SLL_FUNC void sll_init_identifier_list(sll_identifier_list_t* o);



__SLL_FUNC void sll_init_identifier_table(sll_identifier_table_t* o);



__SLL_FUNC void sll_init_input_buffer(sll_input_buffer_t* o);



__SLL_FUNC void sll_init_input_data_stream(sll_input_data_stream_t* o);



__SLL_FUNC void sll_init_binary_heap(sll_binary_heap_t* o);



__SLL_FUNC void sll_init_internal_function(sll_internal_function_t* o);



__SLL_FUNC void sll_init_internal_function_table(sll_internal_function_table_t* o);



__SLL_FUNC void sll_init_json_array(sll_json_array_t* o);



__SLL_FUNC void sll_init_json_map_keypair(sll_json_map_keypair_t* o);



__SLL_FUNC void sll_init_json_map(sll_json_map_t* o);



__SLL_FUNC void sll_init_json_object_data(sll_json_object_data_t* o);



__SLL_FUNC void sll_init_json_object(sll_json_object_t* o);



__SLL_FUNC void sll_init_loop_data(sll_loop_data_t* o);



__SLL_FUNC void sll_init_map(sll_map_t* o);



__SLL_FUNC void sll_init_object_data(sll_object_data_t* o);



__SLL_FUNC void sll_init_object(sll_object_t* o);



__SLL_FUNC void sll_init_output_data_stream(sll_output_data_stream_t* o);



__SLL_FUNC void sll_init_runtime_data(sll_runtime_data_t* o);



__SLL_FUNC void sll_init_runtime_object_data(sll_runtime_object_data_t* o);



__SLL_FUNC void sll_init_runtime_object(sll_runtime_object_t* o);



__SLL_FUNC void sll_init_stack_data(sll_stack_data_t* o);



__SLL_FUNC void sll_init_string(sll_string_t* o);



__SLL_FUNC void sll_init_string_table(sll_string_table_t* o);



#endif

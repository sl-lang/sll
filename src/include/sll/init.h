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
#define SLL_INIT_HANDLE_DATA(o) \
	do{ \
		(o)->t=SLL_HANDLE_UNKNOWN_TYPE; \
		(o)->h=0; \
	} while (0)
#define SLL_INIT_MAP(o) \
	do{ \
		(o)->l=0; \
		(o)->v=NULL; \
	} while (0)
#define SLL_INIT_STRING(o) \
	do{ \
		(o)->l=0; \
		(o)->c=0; \
		(o)->v=NULL; \
	} while (0)

#define SLL_INIT_ARRAY_STRUCT {0,NULL}
#define SLL_INIT_ASSEMBLY_DATA_STRUCT {0,NULL,0,0,{0,NULL},{NULL,0},{NULL,NULL,0,NULL}}
#define SLL_INIT_COMPILATION_DATA_STRUCT {NULL,0,NULL,{.il=NULL,.ill=0},{NULL,0},{NULL,0},{NULL,0},{NULL,NULL,0,NULL,0},0}
#define SLL_INIT_HANDLE_DATA_STRUCT {SLL_HANDLE_UNKNOWN_TYPE,0}
#define SLL_INIT_HANDLE_LIST_STRUCT {NULL,0}
#define SLL_INIT_BINARY_HEAP_STRUCT {NULL,0}
#define SLL_INIT_MAP_STRUCT {0,NULL}
#define SLL_INIT_STRING_STRUCT {0,0,NULL}




__SLL_EXTERNAL void sll_deinit(void);



__SLL_EXTERNAL void sll_deinit_array(sll_array_t* a);



__SLL_EXTERNAL void sll_deinit_assembly_data(sll_assembly_data_t* a_dt);



__SLL_EXTERNAL void sll_deinit_assembly_function_table(sll_assembly_function_table_t* ft);



__SLL_EXTERNAL void sll_deinit_assembly_stack_data(sll_assembly_stack_data_t* a_st);



__SLL_EXTERNAL void sll_deinit_binary_heap(sll_binary_heap_t* il);



__SLL_EXTERNAL void sll_deinit_compilation_data(sll_compilation_data_t* c_dt);



__SLL_EXTERNAL void sll_deinit_compilation_stack_data(sll_compilation_stack_data_t* c_st);



__SLL_EXTERNAL void sll_deinit_export_table(sll_export_table_t* et);



__SLL_EXTERNAL void sll_deinit_function_table(sll_function_table_t* ft);



__SLL_EXTERNAL void sll_deinit_handle_list(sll_handle_list_t* hl);



__SLL_EXTERNAL void sll_deinit_header_list(sll_header_list_t* hl);



__SLL_EXTERNAL void sll_deinit_http_response(sll_http_response_t* r);



__SLL_EXTERNAL void sll_deinit_identifier_table(sll_identifier_table_t* idt);



__SLL_EXTERNAL void sll_deinit_internal_function_table(sll_internal_function_table_t* ift);



__SLL_EXTERNAL void sll_deinit_json_array(sll_json_array_t* json_a);



__SLL_EXTERNAL void sll_deinit_json_map_keypair(sll_json_map_keypair_t* json_kp);



__SLL_EXTERNAL void sll_deinit_json_map(sll_json_map_t* json_m);



__SLL_EXTERNAL void sll_deinit_json_object_data(sll_json_object_data_t* json_dt);



__SLL_EXTERNAL void sll_deinit_json_object(sll_json_object_t* json);



__SLL_EXTERNAL void sll_deinit_map(sll_map_t* m);



__SLL_EXTERNAL void sll_deinit_runtime_object(sll_runtime_object_t* o);



__SLL_EXTERNAL void sll_deinit_string(sll_string_t* s);



__SLL_EXTERNAL void sll_deinit_string_table(sll_string_table_t* st);



__SLL_EXTERNAL void sll_init(void);



__SLL_EXTERNAL void sll_init_compilation_data(const sll_char_t* fp,sll_file_t* rf,sll_compilation_data_t* o);



#endif

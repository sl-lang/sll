#ifndef __SLL_FREE_H__
#define __SLL_FREE_H__ 1
#include <sll/common.h>
#include <sll/types.h>



__SLL_FUNC void sll_free_assembly_data(sll_assembly_data_t* a_dt);



__SLL_FUNC void sll_free_assembly_function_table(sll_assembly_function_table_t* ft);



__SLL_FUNC void sll_free_compilation_data(sll_compilation_data_t* c_dt);



__SLL_FUNC void sll_free_export_table(sll_export_table_t* et);



__SLL_FUNC void sll_free_function_table(sll_function_table_t* ft);



__SLL_FUNC void sll_free_identifier_table(sll_identifier_table_t* idt);



__SLL_FUNC void sll_free_string_table(sll_string_table_t* st);



#endif

#ifndef __SLL_IFT_H__
#define __SLL_IFT_H__ 1
#include <sll/common.h>
#include <sll/types.h>



__SLL_FUNC void sll_create_internal_function_table(sll_internal_function_table_t* restrict o);



__SLL_FUNC void sll_free_internal_function_table(sll_internal_function_table_t* restrict ift);



__SLL_FUNC __SLL_RETURN_FUNCTION_INDEX sll_lookup_internal_function(const sll_internal_function_table_t* restrict i_ft,const char* restrict nm);



__SLL_FUNC sll_function_index_t sll_register_internal_function(sll_internal_function_table_t* restrict i_ft,const char* restrict nm,sll_internal_function_pointer_t f);



__SLL_FUNC void sll_register_standard_internal_functions(sll_internal_function_table_t* restrict i_ft);



#endif

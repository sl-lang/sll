#ifndef __SLL_IFT_H__
#define __SLL_IFT_H__ 1
#include <sll/common.h>
#include <sll/types.h>



__SLL_FUNC void sll_create_internal_function_table(sll_internal_function_table_t* o);



__SLL_FUNC void sll_free_internal_function_table(sll_internal_function_table_t* ift);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_function_index_t sll_lookup_internal_function(const sll_internal_function_table_t* i_ft,const sll_char_t* nm);



__SLL_FUNC sll_function_index_t sll_register_internal_function(sll_internal_function_table_t* i_ft,const sll_char_t* nm,sll_internal_function_pointer_t f);



__SLL_FUNC void sll_register_standard_internal_functions(sll_internal_function_table_t* i_ft);



#endif

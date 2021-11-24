#ifndef __SLL_IFT_H__
#define __SLL_IFT_H__ 1
#include <sll/common.h>
#include <sll/types.h>



#define SLL_INTERNAL_FUNCTION_FLAG_REQUIRED 1
#define SLL_INTERNAL_FUNCTION_FLAG_COMPILATION_CALL 2

#define SLL_MAX_FUNCTION_INDEX 0xfffffffe
#define SLL_UNKNOWN_INTERNAL_FUNCTION_INDEX 0xffffffff



__SLL_EXTERNAL void sll_create_internal_function_table(sll_internal_function_table_t* o);



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_function_index_t sll_lookup_internal_function(const sll_internal_function_table_t* i_ft,const sll_char_t* nm);



__SLL_EXTERNAL void sll_register_builtin_internal_functions(sll_internal_function_table_t* i_ft);



__SLL_EXTERNAL sll_function_index_t sll_register_internal_function(sll_internal_function_table_t* i_ft,const sll_char_t* nm,sll_internal_function_pointer_t f,sll_internal_function_type_t t);



#endif

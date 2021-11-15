#ifndef __SLL_UTIL_H__
#define __SLL_UTIL_H__ 1
#include <sll/common.h>
#include <sll/types.h>



#define SLL_COMPARE_RESULT_BELOW 0
#define SLL_COMPARE_RESULT_EQUAL 1
#define SLL_COMPARE_RESULT_ABOVE 2

#define SLL_SANDBOX_FLAG_DISABLE_FILE_IO 1
#define SLL_SANDBOX_FLAG_ENABLE_STDIN_IO 2
#define SLL_SANDBOX_FLAG_ENABLE_STDOUT_IO 4



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_index_t sll_add_string(sll_string_table_t* st,sll_string_t* s,sll_bool_t d);



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_index_t sll_add_string_runtime(sll_string_table_t* st,sll_runtime_object_t* v);



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_compare_result_t sll_compare_data(const void* a,const void* b,sll_size_t l);



__SLL_EXTERNAL void sll_copy_data(const void* s,sll_size_t l,void* d);



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_index_t sll_create_string(sll_string_table_t* st,const sll_char_t* dt,sll_string_length_t l);



__SLL_EXTERNAL sll_bool_t sll_get_sandbox_flag(sll_sandbox_flags_t f);



__SLL_EXTERNAL void sll_set_memory(void* p,sll_size_t l,sll_char_t v);



__SLL_EXTERNAL sll_sandbox_flags_t sll_set_sandbox_flags(sll_sandbox_flags_t f);



__SLL_EXTERNAL void sll_register_cleanup(sll_cleanup_function_t f);



__SLL_EXTERNAL void sll_zero_memory(void* p,sll_size_t l);



#endif

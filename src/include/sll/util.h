#ifndef __SLL_UTIL_H__
#define __SLL_UTIL_H__ 1
#include <sll/common.h>
#include <sll/types.h>



#define SLL_RETURN_ERROR 0
#define SLL_RETURN_NO_ERROR 1

#define SLL_CLEANUP_ORDER_NORMAL 0
#define SLL_CLEANUP_ORDER_RESERVED0 1



__SLL_FUNC __SLL_CHECK_OUTPUT sll_string_index_t sll_add_string(sll_string_table_t* st,sll_string_t* s,sll_bool_t d);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_string_index_t sll_create_string(sll_string_table_t* st,const sll_char_t* dt,sll_string_length_t l);



__SLL_FUNC void sll_deinit(void);



__SLL_FUNC void sll_init(void);



__SLL_FUNC void sll_init_compilation_data(const sll_char_t* fp,sll_input_data_stream_t* is,sll_compilation_data_t* o);



__SLL_FUNC void sll_register_cleanup(sll_cleanup_function f,sll_cleanup_type_t t);



__SLL_FUNC void sll_setup_stack(sll_stack_data_t* o,sll_char_t* bf,sll_stack_offset_t sz);



#endif

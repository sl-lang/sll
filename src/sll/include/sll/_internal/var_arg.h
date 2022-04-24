#ifndef __SLL__INTERNAL_VAR_ARG_H__
#define __SLL__INTERNAL_VAR_ARG_H__ 1
#include <sll/types.h>



#define VAR_ARG_LIST_TYPE_STRUCT SLL_VAR_ARG_LIST_TYPE_RESERVED0



typedef sll_object_t* (*converter_func_t)(void*);



sll_object_t* _var_arg_converter(sll_var_arg_list_t* va);



addr_t _var_arg_get_pointer(sll_var_arg_list_t* va);



#endif

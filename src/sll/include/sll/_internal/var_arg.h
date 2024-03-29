#ifndef __SLL__INTERNAL_VAR_ARG_H__
#define __SLL__INTERNAL_VAR_ARG_H__ 1
#include <sll/gc.h>
#include <sll/types.h>
#include <sll/var_arg.h>



#define VAR_ARG_LIST_TYPE_STRUCT SLL_VAR_ARG_LIST_TYPE_RESERVED0



typedef sll_object_t (*converter_func_t)(void* arg);



sll_object_t _var_arg_converter(sll_var_arg_list_t* va);



float _var_arg_get_float32(sll_var_arg_list_t* va);



addr_t _var_arg_get_pointer(sll_var_arg_list_t* va);



sll_size_t _var_arg_get_mask(sll_var_arg_list_t* va);



#endif

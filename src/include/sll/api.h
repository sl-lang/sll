#ifndef __SLL_API_H__
#define __SLL_API_H__ 1
#include <sll/api/_generated.h>
#include <sll/common.h>
#include <sll/types.h>



#define __API_FUNC_DECL(nm) __SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_api_##nm##_raw(const sll_runtime_object_t*const* al,const sll_arg_count_t all);__SLL_FUNC __SLL_API_TYPE_sll_api_##nm sll_api_##nm(__SLL_API_ARGS_sll_api_##nm)



#endif

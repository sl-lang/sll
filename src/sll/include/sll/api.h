#ifndef __SLL_API_H__
#define __SLL_API_H__ 1
#include <sll/common.h>
#include <sll/generated/api.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Raw API
 * \group raw-api
 * \desc Docs!
 */



#define __API_FUNC_DECL(nm) __SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_api_##nm##_raw(sll_object_t*const* al,sll_arg_count_t all);__SLL_EXTERNAL __SLL_API_CALL __SLL_API_TYPE_sll_api_##nm sll_api_##nm(__SLL_API_ARGS_sll_api_##nm)



#endif

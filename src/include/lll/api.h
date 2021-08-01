#ifndef __LLL_FUNC_H__
#define __LLL_FUNC_H__ 1
#include <lll/common.h>
#include <lll/types.h>



__LLL_FUNC void lll_api_get_time(lll_runtime_object_t* o,lll_arg_count_t ac,lll_runtime_object_t* a);



__LLL_FUNC __LLL_RETURN_COMPARE lll_compare_runtime_object(const lll_runtime_object_t* a,const lll_runtime_object_t* b);



__LLL_FUNC __LLL_RETURN_COMPARE lll_runtime_object_nonzero(const lll_runtime_object_t* o);



#endif

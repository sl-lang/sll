#include <sll/api.h>
#include <sll/api/math.h>
#include <sll/common.h>
#include <sll/internal/api.h>
#include <sll/object.h>
#include <sll/platform/util.h>
#include <sll/static_object.h>
#include <sll/types.h>



__API_FUNC(time_current){
	return sll_platform_get_current_time()*1e-9;
}



__API_FUNC(time_current_ns){
	return sll_platform_get_current_time();
}



__API_FUNC(time_sleep){
	sll_time_t st=sll_platform_get_current_time();
	sll_platform_sleep((SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_INT?a->dt.i*1000000000:sll_api_math_round(a->dt.f*1e9)));
	return (sll_platform_get_current_time()-st)*1e-9;
}



__API_FUNC(time_sleep_ns){
	sll_time_t st=sll_platform_get_current_time();
	sll_platform_sleep((SLL_OBJECT_GET_TYPE(a)==SLL_OBJECT_TYPE_INT?a->dt.i:sll_api_math_round(a->dt.f)));
	return sll_platform_get_current_time()-st;
}

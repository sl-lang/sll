#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/common.h>
#include <sll/constants.h>
#include <sll/operator.h>
#include <sll/platform.h>
#include <sll/static_object.h>
#include <sll/types.h>



__API_FUNC(time_current){
	return sll_platform_get_current_time()*1e-9;
}



__API_FUNC(time_current_nanos){
	return sll_platform_get_current_time();
}



__API_FUNC(time_sleep){
	sll_time_t st=sll_platform_get_current_time();
	sll_platform_sleep((SLL_RUNTIME_OBJECT_GET_TYPE(a)==SLL_RUNTIME_OBJECT_TYPE_INT?a->dt.i*1000000000:SLL_ROUND_FLOAT(a->dt.f*1e9)));
	return (sll_platform_get_current_time()-st)*1e-9;
}



__API_FUNC(time_sleep_nanos){
	sll_time_t st=sll_platform_get_current_time();
	sll_platform_sleep((SLL_RUNTIME_OBJECT_GET_TYPE(a)==SLL_RUNTIME_OBJECT_TYPE_INT?a->dt.i:SLL_ROUND_FLOAT(a->dt.f)));
	return sll_platform_get_current_time()-st;
}

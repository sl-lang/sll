#include <sll/api/math.h>
#include <sll/common.h>
#include <sll/object.h>
#include <sll/parse_args.h>
#include <sll/platform/util.h>
#include <sll/static_object.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_float_t sll_api_time_current(void){
	return sll_platform_get_current_time()*1e-9;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_time_current_ns(void){
	return sll_platform_get_current_time();
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_float_t sll_api_time_sleep(sll_int_float_t* time){
	sll_time_t st=sll_platform_get_current_time();
	sll_platform_sleep((time->t==SLL_PARSE_ARGS_TYPE_INT?time->dt.i*1000000000:sll_api_math_round(time->dt.f*1e9)));
	return (sll_platform_get_current_time()-st)*1e-9;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_time_sleep_ns(sll_int_float_t* time){
	sll_time_t st=sll_platform_get_current_time();
	sll_platform_sleep((time->t==SLL_PARSE_ARGS_TYPE_INT?time->dt.i:sll_api_math_round(time->dt.f)));
	return sll_platform_get_current_time()-st;
}

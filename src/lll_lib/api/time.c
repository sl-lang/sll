#include <lll/_lll_internal.h>
#include <lll/api.h>
#include <lll/common.h>
#include <lll/platform.h>
#include <lll/types.h>



__LLL_FUNC void lll_api_get_time(lll_runtime_object_t* o,lll_arg_count_t ac,lll_runtime_object_t* a){
	o->t=LLL_RUNTIME_OBJECT_TYPE_INT;
	o->dt.i=lll_platform_get_current_time();
}



__LLL_FUNC void lll_api_get_time_float(lll_runtime_object_t* o,lll_arg_count_t ac,lll_runtime_object_t* a){
	o->t=LLL_RUNTIME_OBJECT_TYPE_FLOAT;
	o->dt.f=lll_platform_get_current_time()*1e-9;
}



INTERNAL_FUNCTION("time_current",lll_api_get_time_float)
INTERNAL_FUNCTION("time_current_nanos",lll_api_get_time)

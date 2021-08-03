#include <lll/_lll_internal.h>
#include <lll/api.h>
#include <lll/constants.h>
#include <lll/platform.h>
#include <lll/types.h>



__API_FUNC(time_current){
	o->t=LLL_RUNTIME_OBJECT_TYPE_FLOAT;
	o->dt.f=lll_platform_get_current_time()*1e-9;
}



__API_FUNC(time_current_nanos){
	o->t=LLL_RUNTIME_OBJECT_TYPE_INT;
	o->dt.i=lll_platform_get_current_time();
}

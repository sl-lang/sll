#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/constants.h>
#include <sll/platform.h>
#include <sll/static_object.h>
#include <sll/types.h>



__API_FUNC(time_current){
	return SLL_FROM_FLOAT(sll_platform_get_current_time()*1e-9);
}



__API_FUNC(time_current_nanos){
	return SLL_FROM_INT(sll_platform_get_current_time());
}



__API_FUNC(time_sleep){
	sll_time_t st=sll_platform_get_current_time();
	if (a->t==SLL_RUNTIME_OBJECT_TYPE_INT){
		sll_platform_sleep(a->dt.i*1000000000);
	}
	else{
		sll_platform_sleep((sll_time_t)(a->dt.f*1e9));
	}
	return SLL_FROM_FLOAT((sll_platform_get_current_time()-st)*1e-9);
}



__API_FUNC(time_sleep_nanos){
	sll_time_t st=sll_platform_get_current_time();
	if (a->t==SLL_RUNTIME_OBJECT_TYPE_INT){
		sll_platform_sleep(a->dt.i);
	}
	else{
		sll_platform_sleep((sll_time_t)a->dt.f);
	}
	return SLL_FROM_INT(sll_platform_get_current_time()-st);
}

#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/constants.h>
#include <sll/platform.h>
#include <sll/static_object.h>
#include <sll/types.h>



__API_FUNC(time_current){
	return sll_float_to_object(sll_platform_get_current_time()*1e-9);
}



__API_FUNC(time_current_nanos){
	return sll_int_to_object(sll_platform_get_current_time());
}



__API_FUNC(time_sleep){
	sll_time_t st=sll_platform_get_current_time();
	if (ac){
		const sll_runtime_object_t* v=*a;
		if (v->t==SLL_RUNTIME_OBJECT_TYPE_INT){
			sll_platform_sleep(v->dt.i*1000000000);
		}
		else if (v->t==SLL_RUNTIME_OBJECT_TYPE_FLOAT){
			sll_platform_sleep((sll_time_t)(v->dt.f*1e9));
		}
	}
	return sll_float_to_object((sll_platform_get_current_time()-st)*1e-9);
}



__API_FUNC(time_sleep_nanos){
	sll_time_t st=sll_platform_get_current_time();
	if (ac){
		const sll_runtime_object_t* v=*a;
		if (v->t==SLL_RUNTIME_OBJECT_TYPE_INT){
			sll_platform_sleep(v->dt.i);
		}
		else if (v->t==SLL_RUNTIME_OBJECT_TYPE_FLOAT){
			sll_platform_sleep((sll_time_t)v->dt.f);
		}
	}
	return sll_int_to_object(sll_platform_get_current_time()-st);
}

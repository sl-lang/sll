#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/constants.h>
#include <sll/platform.h>
#include <sll/types.h>



__API_FUNC(time_current){
	o->t=SLL_RUNTIME_OBJECT_TYPE_FLOAT;
	o->dt.f=sll_platform_get_current_time()*1e-9;
}



__API_FUNC(time_current_nanos){
	o->t=SLL_RUNTIME_OBJECT_TYPE_INT;
	o->dt.i=sll_platform_get_current_time();
}



__API_FUNC(time_sleep){
	sll_time_t st=sll_platform_get_current_time();
	if (ac){
		if (a->t==SLL_RUNTIME_OBJECT_TYPE_INT){
			sll_platform_sleep(a->dt.i*1000000000);
		}
		else if (a->t==SLL_RUNTIME_OBJECT_TYPE_FLOAT){
			sll_platform_sleep((sll_time_t)(a->dt.f*1e9));
		}
	}
	o->t=SLL_RUNTIME_OBJECT_TYPE_FLOAT;
	o->dt.f=(sll_platform_get_current_time()-st)*1e-9;
}



__API_FUNC(time_sleep_nanos){
	sll_time_t st=sll_platform_get_current_time();
	if (ac){
		if (a->t==SLL_RUNTIME_OBJECT_TYPE_INT){
			sll_platform_sleep(a->dt.i);
		}
		else if (a->t==SLL_RUNTIME_OBJECT_TYPE_FLOAT){
			sll_platform_sleep((sll_time_t)a->dt.f);
		}
	}
	o->t=SLL_RUNTIME_OBJECT_TYPE_INT;
	o->dt.i=sll_platform_get_current_time()-st;
}

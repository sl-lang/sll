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

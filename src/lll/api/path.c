#include <lll/_lll_internal.h>
#include <lll/api.h>
#include <lll/constants.h>
#include <lll/platform.h>
#include <lll/string.h>
#include <lll/types.h>
#include <stdlib.h>



__API_FUNC(path_absolute){
	o->t=LLL_RUNTIME_OBJECT_TYPE_STRING;
	if (!ac||a->t!=LLL_RUNTIME_OBJECT_TYPE_STRING){
		o->dt.s=&_zero_string;
	}
	else{
		char bf[LLL_API_MAX_FILE_PATH_LENGTH];
		lll_string_length_t l=(lll_string_length_t)lll_platform_path_absolute((char*)a->dt.s->v,(lll_buffer_t)bf,LLL_API_MAX_FILE_PATH_LENGTH);
		if (!l){
			o->dt.s=a->dt.s;
		}
		else{
			o->dt.s=lll_string_create(l);
			o->dt.s->rc=0;
			o->dt.s->c=0;
			for (lll_string_length_t i=0;i<l;i++){
				o->dt.s->c^=*(bf+i);
				o->dt.s->v[i]=*(bf+i);
			}
			o->dt.s->v[l]=0;
		}
	}
	o->dt.s->rc++;
}



__API_FUNC(path_exists){
	o->t=LLL_RUNTIME_OBJECT_TYPE_INT;
	if (!ac||a->t!=LLL_RUNTIME_OBJECT_TYPE_STRING){
		o->dt.i=0;
	}
	else{
		o->dt.i=lll_platform_path_exists((char*)a->dt.s->v);
	}
}



__API_FUNC(path_is_dir){
	o->t=LLL_RUNTIME_OBJECT_TYPE_INT;
	if (!ac||a->t!=LLL_RUNTIME_OBJECT_TYPE_STRING){
		o->dt.i=0;
	}
	else{
		o->dt.i=lll_platform_path_is_directory((char*)a->dt.s->v);
	}
}



__API_FUNC(path_relative){
	o->t=LLL_OBJECT_TYPE_STRING;
	o->dt.s=&_zero_string;
	o->dt.s->rc++;
}



__API_FUNC(path_size){
	o->t=LLL_RUNTIME_OBJECT_TYPE_INT;
	o->dt.i=0;
}

#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/constants.h>
#include <sll/platform.h>
#include <sll/string.h>
#include <sll/types.h>
#include <stdlib.h>



__API_FUNC(path_absolute){
	o->t=SLL_RUNTIME_OBJECT_TYPE_STRING;
	if (!ac||a->t!=SLL_RUNTIME_OBJECT_TYPE_STRING){
		o->dt.s=&_zero_string;
	}
	else{
		char bf[SLL_API_MAX_FILE_PATH_LENGTH];
		sll_string_length_t l=(sll_string_length_t)sll_platform_path_absolute((char*)a->dt.s->v,(sll_buffer_t)bf,SLL_API_MAX_FILE_PATH_LENGTH);
		if (!l){
			o->dt.s=a->dt.s;
		}
		else{
			o->dt.s=sll_string_create(l);
			o->dt.s->rc=0;
			o->dt.s->c=0;
			for (sll_string_length_t i=0;i<l;i++){
				o->dt.s->c^=*(bf+i);
				o->dt.s->v[i]=*(bf+i);
			}
			o->dt.s->v[l]=0;
		}
	}
	o->dt.s->rc++;
}



__API_FUNC(path_exists){
	o->t=SLL_RUNTIME_OBJECT_TYPE_INT;
	if (!ac||a->t!=SLL_RUNTIME_OBJECT_TYPE_STRING){
		o->dt.i=0;
	}
	else{
		o->dt.i=sll_platform_path_exists((char*)a->dt.s->v);
	}
}



__API_FUNC(path_is_dir){
	o->t=SLL_RUNTIME_OBJECT_TYPE_INT;
	if (!ac||a->t!=SLL_RUNTIME_OBJECT_TYPE_STRING){
		o->dt.i=0;
	}
	else{
		o->dt.i=sll_platform_path_is_directory((char*)a->dt.s->v);
	}
}



__API_FUNC(path_relative){
	o->t=SLL_OBJECT_TYPE_STRING;
	o->dt.s=&_zero_string;
	o->dt.s->rc++;
}



__API_FUNC(path_size){
	o->t=SLL_RUNTIME_OBJECT_TYPE_INT;
	o->dt.i=0;
}

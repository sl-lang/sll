#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/constants.h>
#include <sll/gc.h>
#include <sll/platform.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>



__API_FUNC(path_absolute){
	if (!ac){
		SLL_RETURN_ZERO_STRING;
	}
	const sll_runtime_object_t* v=*a;
	if (v->t!=SLL_RUNTIME_OBJECT_TYPE_STRING){
		SLL_RETURN_ZERO_STRING;
	}
	sll_char_t bf[SLL_API_MAX_FILE_PATH_LENGTH];
	sll_string_length_t l=(sll_string_length_t)sll_platform_path_absolute((char*)v->dt.s.v,bf,SLL_API_MAX_FILE_PATH_LENGTH);
	if (!l){
		sll_runtime_object_t* o=SLL_CREATE();
		o->rc=1;
		o->t=SLL_RUNTIME_OBJECT_TYPE_STRING;
		sll_string_clone(&(v->dt.s),&(o->dt.s));
		return o;
	}
	sll_runtime_object_t* o=SLL_CREATE();
	o->rc=1;
	o->t=SLL_RUNTIME_OBJECT_TYPE_STRING;
	sll_string_create(l,&(o->dt.s));
	for (sll_string_length_t i=0;i<l;i++){
		o->dt.s.c^=*(bf+i);
		o->dt.s.v[i]=*(bf+i);
	}
	return o;
}



__API_FUNC(path_exists){
	if (!ac){
		SLL_RETURN_ZERO;
	}
	const sll_runtime_object_t* v=*a;
	if (v->t!=SLL_RUNTIME_OBJECT_TYPE_STRING){
		SLL_RETURN_ZERO;
	}
	if (sll_platform_path_exists((char*)v->dt.s.v)){
		SLL_RETURN_ONE;
	}
	SLL_RETURN_ZERO;
}



__API_FUNC(path_is_dir){
	if (!ac){
		SLL_RETURN_ZERO;
	}
	const sll_runtime_object_t* v=*a;
	if (v->t!=SLL_RUNTIME_OBJECT_TYPE_STRING){
		SLL_RETURN_ZERO;
	}
	if (sll_platform_path_is_directory((char*)v->dt.s.v)){
		SLL_RETURN_ONE;
	}
	SLL_RETURN_ZERO;
}



__API_FUNC(path_relative){
	SLL_UNIMPLEMENTED();
	SLL_RETURN_ZERO_STRING;
}



__API_FUNC(path_size){
	SLL_UNIMPLEMENTED();
	SLL_RETURN_ZERO;
}

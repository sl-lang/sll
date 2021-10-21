#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/api/path.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/platform.h>
#include <sll/runtime_object.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>



__API_FUNC(path_absolute){
	sll_char_t bf[SLL_API_MAX_FILE_PATH_LENGTH];
	sll_string_length_t l=sll_platform_path_absolute(a->v,bf,SLL_API_MAX_FILE_PATH_LENGTH+1);
	if (!l){
		sll_string_clone(a,out);
		return;
	}
	sll_string_create(l,out);
	for (sll_string_length_t i=0;i<l;i++){
		out->v[i]=*(bf+i);
	}
	sll_string_calculate_checksum(out);
}



__API_FUNC(path_exists){
	return sll_platform_path_exists(a->v);
}



__API_FUNC(path_list_dir){
	sll_string_t* dt=NULL;
	sll_array_length_t l=sll_platform_list_directory(a->v,&dt);
	sll_runtime_object_t* o=SLL_CREATE();
	o->t=SLL_RUNTIME_OBJECT_TYPE_ARRAY;
	sll_array_create(l,&(o->dt.a));
	for (sll_array_length_t i=0;i<l;i++){
		sll_runtime_object_t* n=SLL_CREATE();
		n->t=SLL_RUNTIME_OBJECT_TYPE_STRING;
		n->dt.s=*(dt+i);
		o->dt.a.v[i]=n;
	}
	free(dt);
	return o;
}



__API_FUNC(path_recursive_list_dir){
	sll_string_t* dt=NULL;
	sll_array_length_t l=sll_platform_list_directory_recursive(a->v,&dt);
	sll_runtime_object_t* o=SLL_CREATE();
	o->t=SLL_RUNTIME_OBJECT_TYPE_ARRAY;
	sll_array_create(l,&(o->dt.a));
	for (sll_array_length_t i=0;i<l;i++){
		sll_runtime_object_t* n=SLL_CREATE();
		n->t=SLL_RUNTIME_OBJECT_TYPE_STRING;
		n->dt.s=*(dt+i);
		o->dt.a.v[i]=n;
	}
	free(dt);
	return o;
}



__API_FUNC(path_is_dir){
	return sll_platform_path_is_directory(a->v);
}



__API_FUNC(path_relative){
	SLL_UNIMPLEMENTED();
}



__API_FUNC(path_size){
	return sll_platform_file_size(a->v);
}

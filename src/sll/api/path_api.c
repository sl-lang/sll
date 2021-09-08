#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/constants.h>
#include <sll/gc.h>
#include <sll/platform.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>



__API_FUNC(path_absolute){
	sll_char_t bf[SLL_API_MAX_FILE_PATH_LENGTH];
	sll_string_length_t l=(sll_string_length_t)sll_platform_path_absolute((char*)a->v,bf,SLL_API_MAX_FILE_PATH_LENGTH);
	if (!l){
		sll_string_clone(a,out);
		return;
	}
	sll_string_create(l,out);
	for (sll_string_length_t i=0;i<l;i++){
		out->v[i]=*(bf+i);
	}
	sll_string_hash(out);
}



__API_FUNC(path_exists){
	if (sll_platform_path_exists((char*)a->v)){
		return 1;
	}
	return 0;
}



__API_FUNC(path_is_dir){
	if (sll_platform_path_is_directory((char*)a->v)){
		return 1;
	}
	return 0;
}



__API_FUNC(path_relative){
	SLL_UNIMPLEMENTED();
}



__API_FUNC(path_size){
	SLL_UNIMPLEMENTED();
	return 0;
}

#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/api/path.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/platform.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>



__API_FUNC(path_absolute){
	sll_char_t bf[SLL_STRING_ALIGN_LENGTH(SLL_API_MAX_FILE_PATH_LENGTH)];
	sll_string_length_t l=(sll_string_length_t)sll_platform_path_absolute(a->v,bf,SLL_API_MAX_FILE_PATH_LENGTH+1);
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
	return sll_platform_path_exists(a->v);
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

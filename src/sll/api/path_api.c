#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/api/path.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/file.h>
#include <sll/gc.h>
#include <sll/memory.h>
#include <sll/object.h>
#include <sll/platform.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/util.h>



__SLL_EXTERNAL sll_string_length_t sll_path_split(const sll_string_t* s){
	sll_char_t dt[2]={
		'/',
		'\\'
	};
	sll_string_length_t o=sll_string_index_reverse_multiple(s,dt,2,0);
	return (o==SLL_MAX_STRING_INDEX?0:o+1);
}



__API_FUNC(path_absolute){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_PATH_API)){
		sll_string_clone(a,out);
		return;
	}
	sll_char_t bf[SLL_API_MAX_FILE_PATH_LENGTH];
	sll_string_from_pointer_length(bf,sll_platform_absolute_path(a->v,bf,SLL_API_MAX_FILE_PATH_LENGTH),out);
}



__API_FUNC(path_exists){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_PATH_API)){
		return 0;
	}
	return sll_platform_path_exists(a->v);
}



__API_FUNC(path_get_cwd){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_PATH_API)){
		sll_string_create(0,out);
		return;
	}
	sll_char_t bf[SLL_API_MAX_FILE_PATH_LENGTH];
	sll_string_from_pointer_length(bf,sll_platform_get_current_working_directory(bf,SLL_API_MAX_FILE_PATH_LENGTH),out);
}



__API_FUNC(path_is_dir){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_PATH_API)){
		return 0;
	}
	return sll_platform_path_is_directory(a->v);
}



__API_FUNC(path_join){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_PATH_API)){
		sll_string_create(0,out);
		return;
	}
	sll_char_t bf[SLL_API_MAX_FILE_PATH_LENGTH];
	sll_string_length_t i=0;
	if (ac){
		if ((*a)->l+i>=SLL_API_MAX_FILE_PATH_LENGTH){
			sll_string_create(0,out);
			return;
		}
		i=(*a)->l;
		sll_copy_data((*a)->v,i,bf);
		ac--;
		a++;
		while (ac){
			if (!i||(bf[i-1]!='/'&&bf[i-1]!='\\')){
				bf[i]=SLL_API_FILE_PATH_SEPARATOR;
				i++;
			}
			if ((*a)->l+i>=SLL_API_MAX_FILE_PATH_LENGTH){
				sll_string_create(0,out);
				return;
			}
			sll_copy_data((*a)->v,(*a)->l,bf+i);
			i+=(*a)->l;
			ac--;
			a++;
		}
	}
	sll_string_from_pointer_length(bf,i,out);
}



__API_FUNC(path_list_dir){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_PATH_API)){
		SLL_INIT_ARRAY(out);
		return;
	}
	sll_string_t* dt=NULL;
	sll_array_length_t l=sll_platform_list_directory(a->v,&dt);
	if (!sll_array_create(l,out)){
		SLL_UNIMPLEMENTED();
	}
	for (sll_array_length_t i=0;i<l;i++){
		sll_object_t* n=SLL_CREATE();
		n->t=SLL_OBJECT_TYPE_STRING;
		n->dt.s=*(dt+i);
		out->v[i]=n;
	}
	sll_deallocate(dt);
}



__API_FUNC(path_recursive_list_dir){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_PATH_API)){
		SLL_INIT_ARRAY(out);
		return;
	}
	sll_string_t* dt=NULL;
	sll_array_length_t l=sll_platform_list_directory_recursive(a->v,&dt);
	if (!sll_array_create(l,out)){
		SLL_UNIMPLEMENTED();
	}
	for (sll_array_length_t i=0;i<l;i++){
		sll_object_t* n=SLL_CREATE();
		n->t=SLL_OBJECT_TYPE_STRING;
		n->dt.s=*(dt+i);
		out->v[i]=n;
	}
	sll_deallocate(dt);
}



__API_FUNC(path_relative){
	SLL_UNIMPLEMENTED();
}



__API_FUNC(path_set_cwd){
	return (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_PATH_API)?0:sll_platform_set_current_working_directory(a->v));
}



__API_FUNC(path_size){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_PATH_API)){
		return 0;
	}
	sll_file_descriptor_t fd=sll_platform_file_open(a->v,SLL_FILE_FLAG_READ);
	if (fd==SLL_UNKNOWN_FILE_DESCRIPTOR){
		return 0;
	}
	sll_size_t o=sll_platform_file_size(fd);
	sll_platform_file_close(fd);
	return o;
}



__API_FUNC(path_split){
	sll_string_length_t i=sll_path_split(a);
	if (!sll_array_create(2,out)){
		SLL_UNIMPLEMENTED();
	}
	out->v[0]=SLL_CREATE();
	out->v[0]->t=SLL_OBJECT_TYPE_STRING;
	sll_string_from_pointer_length(a->v,i,&(out->v[0]->dt.s));
	out->v[1]=SLL_CREATE();
	out->v[1]->t=SLL_OBJECT_TYPE_STRING;
	sll_string_from_pointer_length(a->v+i,a->l-i,&(out->v[1]->dt.s));
}

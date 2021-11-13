#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/api/path.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/memory.h>
#include <sll/platform.h>
#include <sll/runtime_object.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/util.h>



__SLL_FUNC sll_string_length_t sll_path_absolute(const sll_char_t* s,sll_char_t* o,sll_string_length_t ol){
	sll_string_length_t i=0;
	if (!s[0]||(s[0]!='/'&&s[0]!='\\'&&(!s[1]||s[0]<'A'||s[0]>'Z'||s[1]!=':'))){
		i=sll_platform_get_current_working_directory(o,ol);
		if (!i){
			i=sll_string_length_unaligned(s);
			if (i>ol){
				i=ol;
			}
			sll_copy_data(s,i,o);
			return i;
		}
		o[i]=SLL_API_FILE_PATH_SEPARATOR;
		i++;
	}
	sll_string_length_t j=0;
	while (s[j]){
		if (s[j]=='.'&&(!i||o[i-1]==SLL_API_FILE_PATH_SEPARATOR)){
			if (!s[j+1]||s[j+1]=='/'||s[j+1]=='\\'){
				j++;
				continue;
			}
			if (s[j+1]=='.'&&(!s[j+2]||s[j+2]=='/'||s[j+2]=='\\')){
				j+=2;
				SLL_ASSERT(i);
				sll_string_length_t k=i;
				i--;
				while (i&&o[i-1]!=SLL_API_FILE_PATH_SEPARATOR){
					i--;
				}
				if (!i){
					i=k;
				}
				continue;
			}
		}
		if (s[j]=='/'||s[j]=='\\'){
			if (i&&o[i-1]==SLL_API_FILE_PATH_SEPARATOR){
				j++;
				continue;
			}
			o[i]=SLL_API_FILE_PATH_SEPARATOR;
		}
		else{
			o[i]=s[j];
		}
		i++;
		j++;
		if (i==ol){
			i=sll_string_length_unaligned(s);
			if (i>ol){
				i=ol;
			}
			sll_copy_data(s,i,o);
			return i;
		}
	}
	if (i==ol){
		i=sll_string_length_unaligned(s);
		if (i>ol){
			i=ol;
		}
		sll_copy_data(s,i,o);
	}
	else{
		o[i]=0;
	}
	return i;
}



__API_FUNC(path_absolute){
	sll_char_t bf[SLL_API_MAX_FILE_PATH_LENGTH];
	sll_string_from_pointer_length(bf,sll_path_absolute(a->v,bf,SLL_API_MAX_FILE_PATH_LENGTH),out);
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
	sll_deallocate(dt);
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
	sll_deallocate(dt);
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

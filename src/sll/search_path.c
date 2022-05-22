#include <sll/api/path.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/environment.h>
#include <sll/memory.h>
#include <sll/platform/path.h>
#include <sll/search_path.h>
#include <sll/string.h>
#include <sll/types.h>



__SLL_EXTERNAL void sll_free_search_path(sll_search_path_t* search_path){
	while (search_path->length){
		search_path->length--;
		sll_free_string(search_path->data+search_path->length);
	}
	sll_deallocate(search_path->data);
	search_path->data=NULL;
}



__SLL_EXTERNAL void sll_search_path_create(const sll_string_t* string,sll_search_path_t* out){
	out->length=0;
	out->data=NULL;
	sll_string_length_t i=0;
	while (1){
		sll_string_length_t e=sll_string_index_char(string,SLL_SEARCH_PATH_SPLIT_CHAR,0,i);
		if (e==SLL_MAX_STRING_LENGTH){
			e=string->length;
		}
		out->length++;
		out->data=sll_reallocate(out->data,out->length*sizeof(sll_string_t));
		sll_string_from_pointer_length(string->data+i,e-i,out->data+out->length-1);
		if (e==string->length){
			break;
		}
		i=e+1;
	}
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_search_path_find(const sll_search_path_t* search_path,const sll_string_t* name,sll_search_flags_t flags,sll_string_t* out){
	SLL_INIT_STRING(out);
	if (name->length>=SLL_API_MAX_FILE_PATH_LENGTH){
		return 0;
	}
	if ((flags&SLL_SEARCH_PATH_FLAG_BEFORE)&&sll_platform_path_exists(name->data)){
		sll_string_clone(name,out);
		return 1;
	}
	if (name->length==SLL_API_MAX_FILE_PATH_LENGTH-1){
		return 0;
	}
	sll_char_t bf[SLL_API_MAX_FILE_PATH_LENGTH];
	sll_copy_data(name->data,name->length+1,bf+SLL_API_MAX_FILE_PATH_LENGTH-name->length-1);
	sll_string_length_t sz=SLL_API_MAX_FILE_PATH_LENGTH-name->length-2;
	if (!sz){
		return 0;
	}
	bf[sz]=SLL_API_FILE_PATH_SEPARATOR;
	for (sll_search_path_length_t i=0;i<search_path->length;i++){
		if ((search_path->data+i)->length<=sz){
			sll_copy_data((search_path->data+i)->data,(search_path->data+i)->length,bf+sz-(search_path->data+i)->length);
			if (sll_platform_path_exists(bf+sz-(search_path->data+i)->length)){
				const sll_string_t* dt[2]={
					search_path->data+i,
					name
				};
				sll_api_path_join(dt,2,out);
				return 1;
			}
		}
	}
	if ((flags&SLL_SEARCH_PATH_FLAG_AFTER)&&sll_platform_path_exists(name->data)){
		sll_string_clone(name,out);
		return 1;
	}
	return 0;
}



__SLL_EXTERNAL void sll_search_path_from_environment(const sll_string_t* key,sll_search_path_t* out){
	sll_string_t tmp;
	sll_expand_environment_variable(key,&tmp);
	sll_search_path_create(&tmp,out);
	sll_free_string(&tmp);
}

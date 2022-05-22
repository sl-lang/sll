#include <sll/api/path.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/environment.h>
#include <sll/memory.h>
#include <sll/platform/path.h>
#include <sll/search_path.h>
#include <sll/string.h>
#include <sll/types.h>



__SLL_EXTERNAL void sll_free_search_path(sll_search_path_t* sp){
	while (sp->length){
		sp->length--;
		sll_free_string(sp->data+sp->length);
	}
	sll_deallocate(sp->data);
	sp->data=NULL;
}



__SLL_EXTERNAL void sll_search_path_create(const sll_string_t* src,sll_search_path_t* o){
	o->length=0;
	o->data=NULL;
	sll_string_length_t i=0;
	while (1){
		sll_string_length_t e=sll_string_index_char(src,SLL_SEARCH_PATH_SPLIT_CHAR,0,i);
		if (e==SLL_MAX_STRING_LENGTH){
			e=src->length;
		}
		o->length++;
		o->data=sll_reallocate(o->data,o->length*sizeof(sll_string_t));
		sll_string_from_pointer_length(src->data+i,e-i,o->data+o->length-1);
		if (e==src->length){
			break;
		}
		i=e+1;
	}
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_search_path_find(const sll_search_path_t* sp,const sll_string_t* nm,sll_search_flags_t fl,sll_string_t* o){
	SLL_INIT_STRING(o);
	if (nm->length>=SLL_API_MAX_FILE_PATH_LENGTH){
		return 0;
	}
	if ((fl&SLL_SEARCH_PATH_FLAG_BEFORE)&&sll_platform_path_exists(nm->data)){
		sll_string_clone(nm,o);
		return 1;
	}
	if (nm->length==SLL_API_MAX_FILE_PATH_LENGTH-1){
		return 0;
	}
	sll_char_t bf[SLL_API_MAX_FILE_PATH_LENGTH];
	sll_copy_data(nm->data,nm->length+1,bf+SLL_API_MAX_FILE_PATH_LENGTH-nm->length-1);
	sll_string_length_t sz=SLL_API_MAX_FILE_PATH_LENGTH-nm->length-2;
	if (!sz){
		return 0;
	}
	bf[sz]=SLL_API_FILE_PATH_SEPARATOR;
	for (sll_search_path_length_t i=0;i<sp->length;i++){
		if ((sp->data+i)->length<=sz){
			sll_copy_data((sp->data+i)->data,(sp->data+i)->length,bf+sz-(sp->data+i)->length);
			if (sll_platform_path_exists(bf+sz-(sp->data+i)->length)){
				const sll_string_t* dt[2]={
					sp->data+i,
					nm
				};
				sll_api_path_join(dt,2,o);
				return 1;
			}
		}
	}
	if ((fl&SLL_SEARCH_PATH_FLAG_AFTER)&&sll_platform_path_exists(nm->data)){
		sll_string_clone(nm,o);
		return 1;
	}
	return 0;
}



__SLL_EXTERNAL void sll_search_path_from_environment(const sll_string_t* key,sll_search_path_t* o){
	sll_string_t tmp;
	sll_expand_environment_variable(key,&tmp);
	sll_search_path_create(&tmp,o);
	sll_free_string(&tmp);
}

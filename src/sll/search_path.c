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
	while (sp->l){
		sp->l--;
		sll_free_string(sp->dt+sp->l);
	}
	sll_deallocate(sp->dt);
	sp->dt=NULL;
}



__SLL_EXTERNAL void sll_search_path_create(const sll_string_t* src,sll_search_path_t* o){
	o->l=0;
	o->dt=NULL;
	sll_string_length_t i=0;
	while (1){
		sll_string_length_t e=sll_string_index_char(src,SLL_SEARCH_PATH_SPLIT_CHAR,0,i);
		if (e==SLL_MAX_STRING_LENGTH){
			e=src->l;
		}
		o->l++;
		o->dt=sll_reallocate(o->dt,o->l*sizeof(sll_string_t));
		sll_string_from_pointer_length(src->v+i,e-i,o->dt+o->l-1);
		if (e==src->l){
			break;
		}
		i=e+1;
	}
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_search_path_find(const sll_search_path_t* sp,const sll_string_t* nm,sll_flags_t fl,sll_string_t* o){
	SLL_INIT_STRING(o);
	if (nm->l>=SLL_API_MAX_FILE_PATH_LENGTH){
		return 0;
	}
	if ((fl&SLL_SEARCH_PATH_FLAG_BEFORE)&&sll_platform_path_exists(nm->v)){
		sll_string_clone(nm,o);
		return 1;
	}
	if (nm->l==SLL_API_MAX_FILE_PATH_LENGTH-1){
		return 0;
	}
	sll_char_t bf[SLL_API_MAX_FILE_PATH_LENGTH];
	sll_copy_data(nm->v,nm->l+1,bf+SLL_API_MAX_FILE_PATH_LENGTH-nm->l-1);
	sll_string_length_t sz=SLL_API_MAX_FILE_PATH_LENGTH-nm->l-2;
	if (!sz){
		return 0;
	}
	bf[sz]=SLL_API_FILE_PATH_SEPARATOR;
	for (sll_search_path_length_t i=0;i<sp->l;i++){
		if ((sp->dt+i)->l<=sz){
			sll_copy_data((sp->dt+i)->v,(sp->dt+i)->l,bf+sz-(sp->dt+i)->l);
			if (sll_platform_path_exists(bf+sz-(sp->dt+i)->l)){
				const sll_string_t* dt[2]={
					sp->dt+i,
					nm
				};
				sll_api_path_join(dt,2,o);
				return 1;
			}
		}
	}
	if ((fl&SLL_SEARCH_PATH_FLAG_AFTER)&&sll_platform_path_exists(nm->v)){
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

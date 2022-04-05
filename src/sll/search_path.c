#include <sll/common.h>
#include <sll/memory.h>
#include <sll/search_path.h>
#include <sll/string.h>
#include <sll/types.h>



__SLL_EXTERNAL void sll_create_search_path(const sll_string_t* src,sll_search_path_t* o){
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



__SLL_EXTERNAL void sll_free_search_path(sll_search_path_t* sp){
	while (sp->l){
		sp->l--;
		sll_free_string(sp->dt+sp->l);
	}
	sll_deallocate(sp->dt);
	sp->dt=NULL;
}

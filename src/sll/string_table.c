#include <sll/common.h>
#include <sll/memory.h>
#include <sll/object.h>
#include <sll/string.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_index_t sll_add_string(sll_string_table_t* st,sll_string_t* s,sll_bool_t d){
	for (sll_string_index_t i=0;i<st->l;i++){
		if (sll_string_equal(st->dt+i,s)){
			if (d){
				sll_free_string(s);
			}
			return i;
		}
	}
	st->l++;
	st->dt=sll_reallocate(st->dt,st->l*sizeof(sll_string_t));
	*(st->dt+st->l-1)=*s;
	return st->l-1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_index_t sll_create_string(sll_string_table_t* st,const sll_char_t* dt,sll_string_length_t l){
	sll_string_t n;
	sll_string_from_pointer_length(dt,l,&n);
	for (sll_string_index_t i=0;i<st->l;i++){
		if (sll_string_equal(st->dt+i,&n)){
			sll_free_string(&n);
			return i;
		}
	}
	st->l++;
	st->dt=sll_reallocate(st->dt,st->l*sizeof(sll_string_t));
	*(st->dt+st->l-1)=n;
	return st->l-1;
}

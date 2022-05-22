#include <sll/_internal/string.h>
#include <sll/common.h>
#include <sll/memory.h>
#include <sll/object.h>
#include <sll/string.h>
#include <sll/string_table.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_index_t sll_add_string(sll_string_table_t* st,sll_string_t* s,sll_bool_t d){
	for (sll_string_index_t i=0;i<st->length;i++){
		if (STRING_EQUAL(st->data+i,s)){
			if (d){
				sll_free_string(s);
			}
			return i;
		}
	}
	st->length++;
	st->data=sll_reallocate(st->data,st->length*sizeof(sll_string_t));
	*(st->data+st->length-1)=*s;
	return st->length-1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_index_t sll_create_string(sll_string_table_t* st,const sll_char_t* dt,sll_string_length_t l){
	sll_string_t n;
	sll_string_from_pointer_length(dt,l,&n);
	for (sll_string_index_t i=0;i<st->length;i++){
		if (STRING_EQUAL(st->data+i,&n)){
			sll_free_string(&n);
			return i;
		}
	}
	st->length++;
	st->data=sll_reallocate(st->data,st->length*sizeof(sll_string_t));
	*(st->data+st->length-1)=n;
	return st->length-1;
}

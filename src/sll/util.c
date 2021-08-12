#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/core.h>
#include <sll/string.h>
#include <sll/types.h>
#include <stdlib.h>
#include <string.h>



sll_string_t _zero_string={
	0,
	1,
	HEAP_SIGNATURE+1,
	0
};



__SLL_FUNC __SLL_RETURN_STRING_INDEX sll_create_string(sll_string_table_t* st,const sll_char_t* dt,sll_string_length_t l){
	sll_string_checksum_t c=0;
	for (sll_string_length_t i=0;i<l;i++){
		c^=(sll_string_checksum_t)(*(dt+i));
	}
	for (sll_string_index_t i=0;i<st->l;i++){
		sll_string_t* s=*(st->dt+i);
		if (s->c==c&&s->l==l){
			for (sll_string_length_t j=0;j<l;j++){
				if (*(dt+j)!=*(s->v+j)){
					goto _check_next_string;
				}
			}
			return i;
		}
_check_next_string:;
	}
	st->l++;
	st->dt=realloc(st->dt,st->l*sizeof(sll_string_t*));
	sll_string_t* s=sll_string_create(l);
	s->rc=1;
	s->c=c;
	memcpy(s->v,dt,l);
	*(st->dt+st->l-1)=s;
	return st->l-1;
}



__SLL_FUNC __SLL_RETURN_SIZE sll_get_object_size(const sll_object_t* o){
	sll_object_offset_t eoff=0;
	while (o->t==SLL_OBJECT_TYPE_NOP||o->t==SLL_OBJECT_TYPE_DEBUG_DATA){
		eoff++;
		o++;
	}
	switch (o->t){
		case SLL_OBJECT_TYPE_UNKNOWN:
		case SLL_OBJECT_TYPE_CHAR:
		case SLL_OBJECT_TYPE_STRING:
		case SLL_OBJECT_TYPE_IDENTIFIER:
		case SLL_OBJECT_TYPE_INT:
		case SLL_OBJECT_TYPE_FLOAT:
			return eoff+1;
		case SLL_OBJECT_TYPE_FUNC:
		case SLL_OBJECT_TYPE_INTERNAL_FUNC:
			{
				sll_object_offset_t off=1;
				sll_arg_count_t l=o->dt.fn.ac;
				while (l){
					l--;
					off+=sll_get_object_size(o+off);
				}
				return off+eoff;
			}
		case SLL_OBJECT_TYPE_OPERATION_LIST:
			{
				sll_object_offset_t off=1;
				sll_statement_count_t l=o->dt.sc;
				while (l){
					l--;
					off+=sll_get_object_size(o+off);
				}
				return off+eoff;
			}
	}
	sll_object_offset_t off=1;
	sll_arg_count_t l=o->dt.ac;
	while (l){
		l--;
		off+=sll_get_object_size(o+off);
	}
	return off+eoff;
}

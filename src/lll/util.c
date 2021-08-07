#include <lll/_lll_internal.h>
#include <lll/common.h>
#include <lll/core.h>
#include <lll/string.h>
#include <lll/types.h>
#include <stdlib.h>



lll_string_t _zero_string={
	0,
	1,
	HEAP_SIGNATURE+1,
	0
};



__LLL_FUNC __LLL_RETURN_STRING_INDEX lll_create_string(lll_string_table_t* st,const lll_char_t* dt,lll_string_length_t l){
	lll_string_checksum_t c=0;
	for (lll_string_length_t i=0;i<l;i++){
		c^=(lll_string_checksum_t)(*(dt+i));
	}
	for (lll_string_index_t i=0;i<st->l;i++){
		lll_string_t* s=*(st->dt+i);
		if (s->c==c&&s->l==l){
			for (lll_string_length_t j=0;j<l;j++){
				if (*(dt+j)!=*(s->v+j)){
					goto _check_next_string;
				}
			}
			return i;
		}
_check_next_string:;
	}
	st->l++;
	st->dt=realloc(st->dt,st->l*sizeof(lll_string_t*));
	lll_string_t* s=lll_string_create(l);
	s->rc=1;
	s->c=c;
	for (lll_string_length_t i=0;i<l;i++){
		s->v[i]=*(dt+i);
	}
	s->v[l]=0;
	*(st->dt+st->l-1)=s;
	return st->l-1;
}



__LLL_FUNC __LLL_RETURN_SIZE lll_get_object_size(const lll_object_t* o){
	lll_object_offset_t eoff=0;
	while (o->t==LLL_OBJECT_TYPE_NOP||o->t==LLL_OBJECT_TYPE_DEBUG_DATA){
		eoff++;
		o++;
	}
	switch (o->t){
		case LLL_OBJECT_TYPE_UNKNOWN:
		case LLL_OBJECT_TYPE_CHAR:
		case LLL_OBJECT_TYPE_STRING:
		case LLL_OBJECT_TYPE_IDENTIFIER:
		case LLL_OBJECT_TYPE_INT:
		case LLL_OBJECT_TYPE_FLOAT:
			return eoff+1;
		case LLL_OBJECT_TYPE_FUNC:
		case LLL_OBJECT_TYPE_INTERNAL_FUNC:
			{
				lll_object_offset_t off=1;
				lll_arg_count_t l=o->dt.fn.ac;
				while (l){
					l--;
					off+=lll_get_object_size(o+off);
				}
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_OPERATION_LIST:
			{
				lll_object_offset_t off=1;
				lll_statement_count_t l=o->dt.sc;
				while (l){
					l--;
					off+=lll_get_object_size(o+off);
				}
				return off+eoff;
			}
	}
	lll_object_offset_t off=1;
	lll_arg_count_t l=o->dt.ac;
	while (l){
		l--;
		off+=lll_get_object_size(o+off);
	}
	return off+eoff;
}

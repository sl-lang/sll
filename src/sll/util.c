#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/constants.h>
#include <sll/gc.h>
#include <sll/platform.h>
#include <sll/string.h>
#include <sll/types.h>
#include <stdlib.h>
#include <string.h>



static char _util_init=0;



void _util_cleanup(void){
	sll_platform_reset_console();
	_sys_free_argv();
	_gc_free_pages();
}



__SLL_FUNC __SLL_RETURN_STRING_INDEX sll_add_string(sll_string_table_t* st,sll_string_t* s){
	for (sll_string_index_t i=0;i<st->l;i++){
		sll_string_t* k=st->dt+i;
		if (k->c==s->c&&k->l==s->l){
			for (sll_string_length_t j=0;j<s->l;j++){
				if (s->v[j]!=k->v[j]){
					goto _check_next_string;
				}
			}
			free(s.v);
			return i;
		}
_check_next_string:;
	}
	st->l++;
	st->dt=realloc(st->dt,st->l*sizeof(sll_string_t));
	*(st->dt+st->l-1)=*s;
	return st->l-1;
}



__SLL_FUNC __SLL_RETURN_STRING_INDEX sll_create_string(sll_string_table_t* st,const sll_char_t* dt,sll_string_length_t l){
	sll_string_checksum_t c=0;
	for (sll_string_length_t i=0;i<l;i++){
		c^=(sll_string_checksum_t)(*(dt+i));
	}
	for (sll_string_index_t i=0;i<st->l;i++){
		sll_string_t* s=st->dt+i;
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
	st->dt=realloc(st->dt,st->l*sizeof(sll_string_t));
	sll_string_t* s=st->dt+st->l-1;
	sll_string_create(l,s);
	s->c=c;
	memcpy(s->v,dt,l);
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
		case SLL_OBJECT_TYPE_INT:
		case SLL_OBJECT_TYPE_FLOAT:
		case SLL_OBJECT_TYPE_STRING:
		case SLL_OBJECT_TYPE_IDENTIFIER:
			return eoff+1;
		case SLL_OBJECT_TYPE_ARRAY:
			{
				sll_object_offset_t off=1;
				sll_array_length_t l=o->dt.al;
				while (l){
					l--;
					off+=sll_get_object_size(o+off);
				}
				return off+eoff;
			}
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
		case SLL_OBJECT_TYPE_FOR:
		case SLL_OBJECT_TYPE_WHILE:
		case SLL_OBJECT_TYPE_LOOP:
			{
				sll_object_offset_t off=1;
				sll_arg_count_t l=o->dt.l.ac;
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



__SLL_FUNC void sll_init(void){
	if (!_util_init){
		_util_init=1;
		sll_platform_setup_console();
		_gc_init();
		atexit(_util_cleanup);
	}
}

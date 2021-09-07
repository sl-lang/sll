#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/constants.h>
#include <sll/core.h>
#include <sll/gc.h>
#include <sll/platform.h>
#include <sll/string.h>
#include <sll/types.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>



static sll_cleanup_function _util_exit_table[MAX_EXIT_TABLE_SIZE]={sll_platform_reset_console};
static uint16_t _util_exit_table_size=1;
static sll_cleanup_function _util_last_cleanup=NULL;



void _util_cleanup(void){
	while (_util_exit_table_size){
		_util_exit_table_size--;
		_util_exit_table[_util_exit_table_size]();
	}
	if (_util_last_cleanup){
		_util_last_cleanup();
	}
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_string_index_t sll_add_string(sll_string_table_t* st,sll_string_t* s){
	for (sll_string_index_t i=0;i<st->l;i++){
		sll_string_t* k=st->dt+i;
		if (k->c==s->c&&k->l==s->l){
			for (sll_string_length_t j=0;j<s->l;j++){
				if (s->v[j]!=k->v[j]){
					goto _check_next_string;
				}
			}
			free(s->v);
			return i;
		}
_check_next_string:;
	}
	st->l++;
	st->dt=realloc(st->dt,st->l*sizeof(sll_string_t));
	*(st->dt+st->l-1)=*s;
	return st->l-1;
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_string_index_t sll_create_string(sll_string_table_t* st,const sll_char_t* dt,sll_string_length_t l){
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
	memcpy(s->v,dt,l);
	sll_string_hash(s);
	return st->l-1;
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_object_offset_t sll_get_object_size(const sll_object_t* o){
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
		case SLL_OBJECT_TYPE_MAP:
			{
				sll_object_offset_t off=1;
				sll_map_length_t l=o->dt.ml;
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



__SLL_FUNC void sll_register_cleanup(sll_cleanup_function f,sll_cleanup_type_t t){
	if (t==CLEANUP_ORDER_LAST){
		SLL_ASSERT(!_util_last_cleanup);
		_util_last_cleanup=f;
		return;
	}
	SLL_ASSERT(_util_exit_table_size<MAX_EXIT_TABLE_SIZE);
	_util_exit_table[_util_exit_table_size]=f;
	_util_exit_table_size++;
}

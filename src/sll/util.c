#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/object.h>
#include <sll/platform.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/util.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>



static sll_cleanup_function_t _util_exit_table[MAX_CLEANUP_TABLE_SIZE];
static uint16_t _util_exit_table_size=0;
static sll_cleanup_function_t _util_last_cleanup[MAX_LAST_CLEANUP_TABLE_SIZE];
static uint8_t _util_last_cleanup_size=0;



static const sll_object_t* _get_object_size(const sll_object_t* o,sll_object_offset_t* sz){
	while (o->t==SLL_OBJECT_TYPE_NOP||o->t==SLL_OBJECT_TYPE_DEBUG_DATA||o->t==OBJECT_TYPE_CHANGE_STACK){
		if (o->t==OBJECT_TYPE_CHANGE_STACK){
			o=o->dt._p;
		}
		else{
			(*sz)++;
			o++;
		}
	}
	switch (o->t){
		case SLL_OBJECT_TYPE_UNKNOWN:
		case SLL_OBJECT_TYPE_CHAR:
		case SLL_OBJECT_TYPE_INT:
		case SLL_OBJECT_TYPE_FLOAT:
		case SLL_OBJECT_TYPE_STRING:
		case SLL_OBJECT_TYPE_IDENTIFIER:
		case SLL_OBJECT_TYPE_FUNCTION_ID:
			(*sz)++;
			return o+1;
		case SLL_OBJECT_TYPE_ARRAY:
			{
				sll_array_length_t l=o->dt.al;
				(*sz)++;
				o++;
				while (l){
					l--;
					o=_get_object_size(o,sz);
				}
				return o;
			}
		case SLL_OBJECT_TYPE_MAP:
			{
				sll_map_length_t l=o->dt.ml;
				(*sz)++;
				o++;
				while (l){
					l--;
					o=_get_object_size(o,sz);
				}
				return o;
			}
		case SLL_OBJECT_TYPE_FUNC:
		case SLL_OBJECT_TYPE_INTERNAL_FUNC:
			{
				sll_arg_count_t l=o->dt.fn.ac;
				(*sz)++;
				o++;
				while (l){
					l--;
					o=_get_object_size(o,sz);
				}
				return o;
			}
		case SLL_OBJECT_TYPE_FOR:
		case SLL_OBJECT_TYPE_WHILE:
		case SLL_OBJECT_TYPE_LOOP:
			{
				sll_arg_count_t l=o->dt.l.ac;
				(*sz)++;
				o++;
				while (l){
					l--;
					o=_get_object_size(o,sz);
				}
				return o;
			}
		case SLL_OBJECT_TYPE_COMMA:
		case SLL_OBJECT_TYPE_OPERATION_LIST:
			{
				sll_arg_count_t l=o->dt.ac;
				(*sz)++;
				o++;
				while (l){
					l--;
					o=_get_object_size(o,sz);
				}
				return o;
			}
	}
	sll_arg_count_t l=o->dt.ac;
	(*sz)++;
	o++;
	while (l){
		l--;
		o=_get_object_size(o,sz);
	}
	return o;
}



void _execute_cleanup(void){
	while (_util_exit_table_size){
		_util_exit_table_size--;
		_util_exit_table[_util_exit_table_size]();
	}
	while (_util_last_cleanup_size){
		_util_last_cleanup_size--;
		_util_last_cleanup[_util_last_cleanup_size]();
	}
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_string_index_t sll_add_string(sll_string_table_t* st,sll_string_t* s,sll_bool_t d){
	for (sll_string_index_t i=0;i<st->l;i++){
		if (sll_string_equal(st->dt+i,s)){
			if (d){
				free(s->v);
			}
			return i;
		}
	}
	st->l++;
	st->dt=realloc(st->dt,st->l*sizeof(sll_string_t));
	*(st->dt+st->l-1)=*s;
	return st->l-1;
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_string_index_t sll_create_string(sll_string_table_t* st,const sll_char_t* dt,sll_string_length_t l){
	sll_string_t n;
	sll_string_create(l,&n);
	memcpy(n.v,dt,l);
	sll_string_calculate_checksum(&n);
	for (sll_string_index_t i=0;i<st->l;i++){
		sll_string_t* s=st->dt+i;
		if (s->c==n.c&&s->l==l&&!memcmp(dt,s->v,l)){
			free(n.v);
			return i;
		}
	}
	st->l++;
	st->dt=realloc(st->dt,st->l*sizeof(sll_string_t));
	*(st->dt+st->l-1)=n;
	return st->l-1;
}




__SLL_FUNC __SLL_CHECK_OUTPUT sll_object_offset_t sll_get_object_size(const sll_object_t* o){
	sll_object_offset_t sz=0;
	_get_object_size(o,&sz);
	return sz;
}



__SLL_FUNC void sll_register_cleanup(sll_cleanup_function_t f,sll_cleanup_type_t t){
	if (t==CLEANUP_ORDER_LAST){
		SLL_ASSERT(_util_last_cleanup_size<MAX_LAST_CLEANUP_TABLE_SIZE);
		_util_last_cleanup[_util_last_cleanup_size]=f;
		_util_last_cleanup_size++;
		return;
	}
	SLL_ASSERT(_util_exit_table_size<MAX_CLEANUP_TABLE_SIZE);
	_util_exit_table[_util_exit_table_size]=f;
	_util_exit_table_size++;
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_object_t* sll_skip_object(sll_object_t* o){
	while (o->t==SLL_OBJECT_TYPE_NOP||o->t==SLL_OBJECT_TYPE_DEBUG_DATA||o->t==OBJECT_TYPE_CHANGE_STACK){
		o=(o->t==OBJECT_TYPE_CHANGE_STACK?o->dt._p:o+1);
	}
	switch (o->t){
		case SLL_OBJECT_TYPE_UNKNOWN:
		case SLL_OBJECT_TYPE_CHAR:
		case SLL_OBJECT_TYPE_INT:
		case SLL_OBJECT_TYPE_FLOAT:
		case SLL_OBJECT_TYPE_STRING:
		case SLL_OBJECT_TYPE_IDENTIFIER:
		case SLL_OBJECT_TYPE_FUNCTION_ID:
			return o+1;
		case SLL_OBJECT_TYPE_ARRAY:
			{
				sll_array_length_t l=o->dt.al;
				o++;
				while (l){
					l--;
					o=sll_skip_object(o);
				}
				return o;
			}
		case SLL_OBJECT_TYPE_MAP:
			{
				sll_map_length_t l=o->dt.ml;
				o++;
				while (l){
					l--;
					o=sll_skip_object(o);
				}
				return o;
			}
		case SLL_OBJECT_TYPE_FUNC:
		case SLL_OBJECT_TYPE_INTERNAL_FUNC:
			{
				sll_arg_count_t l=o->dt.fn.ac;
				o++;
				while (l){
					l--;
					o=sll_skip_object(o);
				}
				return o;
			}
		case SLL_OBJECT_TYPE_FOR:
		case SLL_OBJECT_TYPE_WHILE:
		case SLL_OBJECT_TYPE_LOOP:
			{
				sll_arg_count_t l=o->dt.l.ac;
				o++;
				while (l){
					l--;
					o=sll_skip_object(o);
				}
				return o;
			}
		case SLL_OBJECT_TYPE_COMMA:
		case SLL_OBJECT_TYPE_OPERATION_LIST:
			{
				sll_arg_count_t l=o->dt.ac;
				o++;
				while (l){
					l--;
					o=sll_skip_object(o);
				}
				return o;
			}
	}
	sll_arg_count_t l=o->dt.ac;
	o++;
	while (l){
		l--;
		o=sll_skip_object(o);
	}
	return o;
}



__SLL_FUNC __SLL_CHECK_OUTPUT const sll_object_t* sll_skip_object_const(const sll_object_t* o){
	return sll_skip_object((sll_object_t*)o);
}

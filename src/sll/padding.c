#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/constants.h>
#include <sll/types.h>



static sll_object_t* _remove_padding_internal(sll_object_t* o,sll_compilation_data_t* c_dt,sll_object_t** d,sll_object_offset_t* rm){
	while (o->t==SLL_OBJECT_TYPE_NOP||o->t==OBJECT_TYPE_CHANGE_STACK){
		if (o->t==OBJECT_TYPE_CHANGE_STACK){
			o=o->dt._p;
		}
		else{
			o++;
			(*rm)++;
		}
	}
	**d=*o;
	(*d)++;
	if ((*d)->t==OBJECT_TYPE_CHANGE_STACK){
		(*d)=(*d)->dt._p;
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
					o=_remove_padding_internal(o,c_dt,d,rm);
				}
				return o;
			}
		case SLL_OBJECT_TYPE_MAP:
			{
				sll_map_length_t l=o->dt.ml;
				o++;
				while (l){
					l--;
					o=_remove_padding_internal(o,c_dt,d,rm);
				}
				return o;
			}
		case SLL_OBJECT_TYPE_FUNC:
			(*(c_dt->ft.dt+o->dt.fn.id))->off-=*rm;
		case SLL_OBJECT_TYPE_INTERNAL_FUNC:
			{
				sll_arg_count_t l=o->dt.fn.ac;
				o++;
				while (l){
					l--;
					o=_remove_padding_internal(o,c_dt,d,rm);
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
					o=_remove_padding_internal(o,c_dt,d,rm);
				}
				return o;
			}
		case SLL_OBJECT_TYPE_COMMA:
		case SLL_OBJECT_TYPE_OPERATION_LIST:
			{
				sll_statement_count_t l=o->dt.sc;
				o++;
				while (l){
					l--;
					o=_remove_padding_internal(o,c_dt,d,rm);
				}
				return o;
			}
		case SLL_OBJECT_TYPE_DEBUG_DATA:
			return _remove_padding_internal(o+1,c_dt,d,rm);
	}
	sll_arg_count_t l=o->dt.ac;
	o++;
	while (l){
		l--;
		o=_remove_padding_internal(o,c_dt,d,rm);
	}
	return o;
}



__SLL_FUNC void sll_remove_object_padding(sll_compilation_data_t* c_dt,sll_object_t* o){
	sll_object_t* d=o;
	sll_object_offset_t rm=0;
	_remove_padding_internal(o,c_dt,&d,&rm);
}

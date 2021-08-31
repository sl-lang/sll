#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/constants.h>
#include <sll/types.h>



static sll_object_offset_t _remove_padding_internal(sll_object_t* o,sll_compilation_data_t* c_dt,sll_object_offset_t* rm){
	sll_object_offset_t eoff=0;
	while (o->t==SLL_OBJECT_TYPE_NOP){
		eoff++;
		o++;
		(*rm)++;
	}
	*(o-*rm)=*o;
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
					off+=_remove_padding_internal(o+off,c_dt,rm);
				}
				return off+eoff;
			}
		case SLL_OBJECT_TYPE_MAP:
			{
				sll_object_offset_t off=1;
				sll_map_length_t l=o->dt.ml;
				while (l){
					l--;
					off+=_remove_padding_internal(o+off,c_dt,rm);
				}
				return off+eoff;
			}
		case SLL_OBJECT_TYPE_FUNC:
			(*(c_dt->ft.dt+o->dt.fn.id))->off-=*rm;
		case SLL_OBJECT_TYPE_INTERNAL_FUNC:
			{
				sll_object_offset_t off=1;
				sll_arg_count_t l=o->dt.fn.ac;
				while (l){
					l--;
					off+=_remove_padding_internal(o+off,c_dt,rm);
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
					off+=_remove_padding_internal(o+off,c_dt,rm);
				}
				return off+eoff;
			}
		case SLL_OBJECT_TYPE_OPERATION_LIST:
			{
				sll_object_offset_t off=1;
				sll_statement_count_t l=o->dt.sc;
				while (l){
					l--;
					off+=_remove_padding_internal(o+off,c_dt,rm);
				}
				return off+eoff;
			}
		case SLL_OBJECT_TYPE_DEBUG_DATA:
			return _remove_padding_internal(o+1,c_dt,rm)+1+eoff;
	}
	sll_object_offset_t off=1;
	sll_arg_count_t l=o->dt.ac;
	while (l){
		l--;
		off+=_remove_padding_internal(o+off,c_dt,rm);
	}
	return off+eoff;
}



__SLL_FUNC void sll_remove_object_padding(sll_compilation_data_t* c_dt,sll_object_t* o){
	sll_object_offset_t rm=0;
	_remove_padding_internal(o,c_dt,&rm);
}

#include <lll_lib.h>
#include <_lll_lib_internal.h>



lll_object_offset_t _remove_padding_internal(lll_object_t* o,lll_compilation_data_t* c_dt,lll_object_offset_t* rm){
	lll_object_offset_t eoff=0;
	while (o->t==LLL_OBJECT_TYPE_NOP||o->t==LLL_OBJECT_TYPE_DEBUG_DATA){
		eoff++;
		o++;
		(*rm)++;
	}
	*(o-*rm)=*o;
	switch (o->t){
		case LLL_OBJECT_TYPE_UNKNOWN:
		case LLL_OBJECT_TYPE_CHAR:
		case LLL_OBJECT_TYPE_STRING:
		case LLL_OBJECT_TYPE_IDENTIFIER:
		case LLL_OBJECT_TYPE_INT:
		case LLL_OBJECT_TYPE_FLOAT:
		case LLL_OBJECT_TYPE_IMPORT:
			return eoff+1;
		case LLL_OBJECT_TYPE_FUNC:
			{
				(*(c_dt->f_dt.dt+o->dt.fn.id))->off-=*rm;
				lll_object_offset_t off=1;
				lll_arg_count_t l=o->dt.fn.ac;
				while (l){
					l--;
					off+=_remove_padding_internal(o+off,c_dt,rm);
				}
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_OPERATION_LIST:
			{
				lll_object_offset_t off=1;
				lll_statement_count_t l=o->dt.sc;
				while (l){
					l--;
					off+=_remove_padding_internal(o+off,c_dt,rm);
				}
				return off+eoff;
			}
	}
	lll_object_offset_t off=1;
	lll_arg_count_t l=o->dt.ac;
	while (l){
		l--;
		off+=_remove_padding_internal(o+off,c_dt,rm);
	}
	return off+eoff;
}



__LLL_IMPORT_EXPORT void lll_remove_object_padding(lll_compilation_data_t* c_dt,lll_object_t* o){
	lll_object_offset_t rm=0;
	_remove_padding_internal(o,c_dt,&rm);
}

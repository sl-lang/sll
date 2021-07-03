#include <lll_lib.h>
#include <_lll_internal.h>
#include <stdint.h>



uint32_t _optimize_object_internal(lll_object_t* o,lll_error_t* e){
	uint32_t eoff=0;
	while (o->t==LLL_OBJECT_TYPE_NOP){
		eoff+=sizeof(lll_object_type_t);
		o=LLL_GET_OBJECT_AFTER_NOP(o);
	}
	switch (LLL_GET_OBJECT_TYPE(o)){
		case LLL_OBJECT_TYPE_UNKNOWN:
		case LLL_OBJECT_TYPE_NIL:
		case LLL_OBJECT_TYPE_TRUE:
		case LLL_OBJECT_TYPE_FALSE:
			return sizeof(lll_object_t)+eoff;
		case LLL_OBJECT_TYPE_CHAR:
			return sizeof(lll_char_object_t)+eoff;
		case LLL_OBJECT_TYPE_STRING:
			return sizeof(lll_string_object_t)+eoff+((lll_string_object_t*)o)->ln;
		case LLL_OBJECT_TYPE_IDENTIFIER:
			return sizeof(lll_identifier_object_t)+eoff;
		case LLL_OBJECT_TYPE_INT:
			return sizeof(lll_integer_object_t)+eoff;
		case LLL_OBJECT_TYPE_FLOAT:
			return sizeof(lll_float_object_t)+eoff;
		case LLL_OBJECT_TYPE_FUNC:
			{
				uint32_t off=sizeof(lll_function_object_t);
				lll_arg_count_t l=((lll_function_object_t*)o)->ac;
				while (l){
					l--;
					off+=_optimize_object_internal(LLL_GET_OBJECT_ARGUMENT(o,off),e);
				}
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_IMPORT:
			return sizeof(lll_import_object_t)+sizeof(lll_import_index_t)*((lll_import_object_t*)o)->ac+eoff;
		case LLL_OBJECT_TYPE_OPERATION_LIST:
			{
				uint32_t off=sizeof(lll_operation_list_object_t);
				lll_statement_count_t l=((lll_operation_list_object_t*)o)->sc;
				for (lll_statement_count_t i=l;i>0;i--){
					uint32_t st_l=_optimize_object_internal(LLL_GET_OBJECT_STATEMENT(o,off),e);
					lll_object_t* st=LLL_GET_OBJECT_STATEMENT(o,off);
					off+=st_l;
					while (st->t==LLL_OBJECT_TYPE_NOP||LLL_GET_OBJECT_TYPE(st)==LLL_OBJECT_TYPE_DEBUG_DATA){
						if (st->t==LLL_OBJECT_TYPE_NOP){
							st=LLL_GET_OBJECT_AFTER_NOP(st);
							continue;
						}
						st=LLL_GET_DEBUG_OBJECT_CHILD((lll_debug_object_t*)st);
					}
					if (LLL_IS_OBJECT_TYPE_TYPE(st)){
						l--;
						for (uint32_t j=off-st_l;j<off;j+=sizeof(lll_object_type_t)){
							LLL_SET_OBJECT_NOP(o,j);
						}
					}
				}
				if (!l){
					o->t=LLL_OBJECT_TYPE_NIL;
					for (uint32_t i=sizeof(lll_object_t);i<sizeof(lll_operation_list_object_t);i+=sizeof(lll_object_type_t)){
						LLL_SET_OBJECT_NOP(o,i);
					}
					return off+eoff;
				}
				((lll_operation_list_object_t*)o)->sc=l;
				if (l==1){
					for (uint32_t i=0;i<sizeof(lll_operation_list_object_t);i+=sizeof(lll_object_type_t)){
						LLL_SET_OBJECT_NOP(o,i);
					}
					return off+eoff;
				}
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_DEBUG_DATA:
			{
				lll_object_t* c=LLL_GET_DEBUG_OBJECT_CHILD((lll_debug_object_t*)o);
				uint8_t ot=LLL_GET_OBJECT_TYPE(c);
				uint32_t off=_optimize_object_internal(c,e);
				if (off==UINT32_MAX){
					return UINT32_MAX;
				}
				if (LLL_GET_OBJECT_TYPE(c)!=ot){
					for (uint32_t i=0;i<sizeof(lll_debug_object_t);i+=sizeof(lll_object_type_t)){
						LLL_SET_OBJECT_NOP(o,i);
					}
				}
				return sizeof(lll_debug_object_t)+off+eoff;
			}
	}
	uint32_t off=sizeof(lll_operator_object_t);
	lll_arg_count_t l=((lll_operator_object_t*)o)->ac;
	lll_arg_count_t cl=l;
	lll_arg_count_t i=0;
	while (i<cl){
		uint32_t al=_optimize_object_internal(LLL_GET_OBJECT_ARGUMENT(o,off),e);
		if (al==UINT32_MAX){
			return UINT32_MAX;
		}
		lll_object_t* a=LLL_GET_OBJECT_ARGUMENT(o,off);
		off+=al;
		if (LLL_IS_OBJECT_TYPE_MATH(o)){
			while (LLL_GET_OBJECT_TYPE(a)==LLL_OBJECT_TYPE_DEBUG_DATA){
				a=LLL_GET_DEBUG_OBJECT_CHILD((lll_debug_object_t*)a);
			}
			switch (LLL_GET_OBJECT_TYPE(a)){
				case LLL_OBJECT_TYPE_CHAR:
					break;
				case LLL_OBJECT_TYPE_STRING:
					break;
				case LLL_OBJECT_TYPE_INT:
					break;
				case LLL_OBJECT_TYPE_FLOAT:
					break;
				case LLL_OBJECT_TYPE_NIL:
					l--;
					for (uint32_t j=off-al;j<off;j+=sizeof(lll_object_type_t)){
						LLL_SET_OBJECT_NOP(o,j);
					}
					break;
				case LLL_OBJECT_TYPE_TRUE:
					switch (LLL_GET_OBJECT_TYPE(o)){
						case LLL_OBJECT_TYPE_DIV:
						case LLL_OBJECT_TYPE_FLOOR_DIV:
						case LLL_OBJECT_TYPE_MOD:
							if (!i){
								break;
							}
						case LLL_OBJECT_TYPE_MULT:
							l--;
							for (uint32_t j=off-al;j<off;j+=sizeof(lll_object_type_t)){
								LLL_SET_OBJECT_NOP(o,j);
							}
							break;
						case LLL_OBJECT_TYPE_BIT_NOT:
							ASSERT(!"Set Object To -2!",e,UINT32_MAX);
							break;
					}
					break;
				case LLL_OBJECT_TYPE_FALSE:
					switch (LLL_GET_OBJECT_TYPE(o)){
						case LLL_OBJECT_TYPE_ADD:
						case LLL_OBJECT_TYPE_SUB:
						case LLL_OBJECT_TYPE_BIT_OR:
						case LLL_OBJECT_TYPE_BIT_XOR:
							l--;
							for (uint32_t j=off-al;j<off;j+=sizeof(lll_object_type_t)){
								LLL_SET_OBJECT_NOP(o,j);
							}
							break;
						case LLL_OBJECT_TYPE_DIV:
						case LLL_OBJECT_TYPE_FLOOR_DIV:
						case LLL_OBJECT_TYPE_MOD:
							if (!i){
								goto _set_to_0;
							}
							e->t=LLL_ERROR_DIVISION_BY_ZERO;
							return UINT32_MAX;
						case LLL_OBJECT_TYPE_MULT:
						case LLL_OBJECT_TYPE_BIT_AND:
_set_to_0:
							i++;
							while (i<cl){
								i++;
								off+=_get_object_size(LLL_GET_OBJECT_ARGUMENT(o,off));
							}
							ASSERT(!"Set Object to 0!",e,UINT32_MAX);
							return off+eoff;
						case LLL_OBJECT_TYPE_BIT_NOT:
							ASSERT(!"Set Object To -1!",e,UINT32_MAX);
							break;
					}
					break;
			}
		}
		i++;
	}
	if (!l&&LLL_IS_OBJECT_TYPE_MATH(o)){
		ASSERT(!"Set Object to 0!",e,UINT32_MAX);
		return off+eoff;
	}
	((lll_operator_object_t*)o)->ac=l;
	if (l==1){
		if (LLL_IS_OBJECT_TYPE_COMPARE(o)){
			o->t=LLL_OBJECT_TYPE_TRUE;
			for (uint32_t j=sizeof(lll_operator_object_t);j<off;j+=sizeof(lll_object_type_t)){
				LLL_SET_OBJECT_NOP(o,j);
			}
			return off+eoff;
		}
		if (LLL_IS_OBJECT_TYPE_MATH(o)){
			for (uint32_t j=0;j<sizeof(lll_operator_object_t);j+=sizeof(lll_object_type_t)){
				LLL_SET_OBJECT_NOP(o,j);
			}
			return off+eoff;
		}
	}
	return off+eoff;
}



uint32_t _remove_padding_internal(lll_object_t* o,uint32_t* rm,lll_compilation_data_t* c_dt,uint64_t st){
	uint8_t* d=(uint8_t*)o-(*rm);
	uint32_t pad=0;
	while (o->t==LLL_OBJECT_TYPE_NOP){
		pad+=sizeof(lll_object_type_t);
		o=LLL_GET_OBJECT_AFTER_NOP(o);
	}
	uint8_t* s=(uint8_t*)o;
	(*rm)+=pad;
	switch (LLL_GET_OBJECT_TYPE(o)){
		case LLL_OBJECT_TYPE_UNKNOWN:
		case LLL_OBJECT_TYPE_NIL:
		case LLL_OBJECT_TYPE_TRUE:
		case LLL_OBJECT_TYPE_FALSE:
			for (uint32_t i=0;i<sizeof(lll_object_t);i++){
				*(d+i)=*(s+i);
			}
			return sizeof(lll_object_t)+pad;
		case LLL_OBJECT_TYPE_CHAR:
			for (uint32_t i=0;i<sizeof(lll_char_object_t);i++){
				*(d+i)=*(s+i);
			}
			return sizeof(lll_char_object_t)+pad;
		case LLL_OBJECT_TYPE_STRING:
			{
				lll_string_length_t ln=((lll_string_object_t*)o)->ln;
				for (uint32_t i=0;i<sizeof(lll_string_object_t)+ln;i++){
					*(d+i)=*(s+i);
				}
				return sizeof(lll_string_object_t)+ln+pad;
			}
		case LLL_OBJECT_TYPE_IDENTIFIER:
			for (uint32_t i=0;i<sizeof(lll_identifier_object_t);i++){
				*(d+i)=*(s+i);
			}
			return sizeof(lll_identifier_object_t)+pad;
		case LLL_OBJECT_TYPE_INT:
			{
				for (uint32_t i=0;i<sizeof(lll_integer_object_t);i++){
					*(d+i)=*(s+i);
				}
				return sizeof(lll_integer_object_t)+pad;
			}
		case LLL_OBJECT_TYPE_FLOAT:
			{
				for (uint32_t i=0;i<sizeof(lll_float_object_t);i++){
					*(d+i)=*(s+i);
				}
				return sizeof(lll_float_object_t)+pad;
			}
		case LLL_OBJECT_TYPE_FUNC:
			{
				for (uint32_t i=0;i<sizeof(lll_function_object_t);i++){
					*(d+i)=*(s+i);
				}
				(*(c_dt->f_dt.dt+((lll_function_object_t*)o)->id))->off-=(uint32_t)(((uint64_t)(void*)s)-((uint64_t)(void*)d));
				uint32_t off=sizeof(lll_function_object_t);
				lll_arg_count_t l=((lll_operator_object_t*)o)->ac;
				while (l){
					l--;
					off+=_remove_padding_internal(LLL_GET_OBJECT_ARGUMENT(o,off),rm,c_dt,st);
				}
				return off+pad;
			}
		case LLL_OBJECT_TYPE_IMPORT:
			{
				uint32_t sz=sizeof(lll_import_object_t)+sizeof(lll_import_index_t)*((lll_import_object_t*)o)->ac;
				for (uint32_t i=0;i<sz;i++){
					*(d+i)=*(s+i);
				}
				return sz+pad;
			}
		case LLL_OBJECT_TYPE_OPERATION_LIST:
			{
				for (uint32_t i=0;i<sizeof(lll_operation_list_object_t);i++){
					*(d+i)=*(s+i);
				}
				uint32_t off=sizeof(lll_operation_list_object_t);
				lll_statement_count_t l=((lll_operation_list_object_t*)o)->sc;
				while (l){
					l--;
					off+=_remove_padding_internal(LLL_GET_OBJECT_STATEMENT(o,off),rm,c_dt,st);
				}
				return off+pad;
			}
		case LLL_OBJECT_TYPE_DEBUG_DATA:
			for (uint32_t i=0;i<sizeof(lll_debug_object_t);i++){
				*(d+i)=*(s+i);
			}
			return sizeof(lll_debug_object_t)+_remove_padding_internal(LLL_GET_DEBUG_OBJECT_CHILD((lll_debug_object_t*)o),rm,c_dt,st)+pad;
	}
	for (uint32_t i=0;i<sizeof(lll_operator_object_t);i++){
		*(d+i)=*(s+i);
	}
	uint32_t off=sizeof(lll_operator_object_t);
	lll_arg_count_t l=((lll_operator_object_t*)o)->ac;
	while (l){
		l--;
		off+=_remove_padding_internal(LLL_GET_OBJECT_ARGUMENT(o,off),rm,c_dt,st);
	}
	return off+pad;
}



__LLL_IMPORT_EXPORT __LLL_RETURN lll_optimize_object(lll_object_t* o,lll_error_t* e){
	if (!_bf){
		e->t=LLL_ERROR_NO_STACK;
		return LLL_RETURN_ERROR;
	}
	if (_optimize_object_internal(o,e)==UINT32_MAX){
		return LLL_RETURN_ERROR;
	}
	return LLL_RETURN_NO_ERROR;
}



__LLL_IMPORT_EXPORT __LLL_RETURN lll_remove_object_padding(lll_compilation_data_t* c_dt,lll_object_t* o,lll_error_t* e){
	if (!_bf){
		e->t=LLL_ERROR_NO_STACK;
		return LLL_RETURN_ERROR;
	}
	uint32_t rm=0;
	_remove_padding_internal(o,&rm,c_dt,(uint64_t)(void*)(c_dt->h));
	return LLL_RETURN_NO_ERROR;
}

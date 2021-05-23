#ifdef _MSC_VER
#include <intrin.h>
#endif
#include <lll_lib.h>
#include <_lll_internal.h>
#include <stdint.h>



#ifdef _MSC_VER
#pragma intrinsic(__movsb)
#define REPEAT_BYTE_COPY(d,s,sz) __movsb(d,s,sz)
#else
static inline void REPEAT_BYTE_COPY(unsigned char* d,unsigned char* s,size_t n){
	__asm__("rep movsb":"=D"(d),"=S"(s),"=c"(n):"0"(d),"1"(s),"2"(n):"memory");
}
#endif




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
			return sizeof(lll_object_t)+eoff+sizeof(char);
		case LLL_OBJECT_TYPE_STRING:
		case LLL_OBJECT_TYPE_IDENTIFIER:
			return sizeof(lll_object_t)+eoff+sizeof(lll_string_length_t)+LLL_GET_OBJECT_STRING_LENGTH(o);
		case LLL_OBJECT_TYPE_INT:
			return sizeof(lll_object_t)+eoff+LLL_GET_OBJECT_INTEGER_WIDTH(o);
		case LLL_OBJECT_TYPE_FLOAT:
			return sizeof(lll_object_t)+eoff+(LLL_IS_OBJECT_FLOAT64(o)?sizeof(double):sizeof(float));
		case LLL_OBJECT_TYPE_OPERATION_LIST:
			{
				uint32_t off=sizeof(lll_object_t)+sizeof(lll_statement_count_t);
				lll_statement_count_t* l=LLL_GET_OBJECT_STATEMENT_COUNT(o);
				for (lll_statement_count_t i=*l;i>0;i--){
					uint32_t st_l=_optimize_object_internal(LLL_GET_OBJECT_STATEMENT(o,off),e);
					lll_object_t* st=LLL_GET_OBJECT_STATEMENT(o,off);
					off+=st_l;
					while (st->t==LLL_OBJECT_TYPE_NOP||LLL_GET_OBJECT_TYPE(st)==LLL_OBJECT_TYPE_DEBUG_DATA){
						if (st->t==LLL_OBJECT_TYPE_NOP){
							st=LLL_GET_OBJECT_AFTER_NOP(st);
							continue;
						}
						lll_debug_object_t* dbg=(lll_debug_object_t*)st;
						st=LLL_GET_DEBUG_OBJECT_CHILD(dbg,sizeof(lll_debug_object_t)+LLL_GET_DEBUG_OBJECT_LINE_NUMBER_WIDTH(dbg)+LLL_GET_DEBUG_OBJECT_COLUMN_NUMBER_WIDTH(dbg)+LLL_GET_DEBUG_OBJECT_FILE_OFFSET_WIDTH(dbg));
					}
					if (LLL_IS_OBJECT_TYPE_TYPE(st)){
						(*l)--;
						for (uint32_t j=off-st_l;j<off;j++){
							*((lll_object_type_t*)LLL_GET_OBJECT_WITH_OFFSET(o,j))=LLL_OBJECT_TYPE_NOP;
						}
					}
				}
				if (!(*l)){
					o->t=LLL_OBJECT_TYPE_NIL;
					for (uint32_t i=sizeof(lll_object_t);i<sizeof(lll_object_t)+sizeof(lll_statement_count_t);i++){
						*((lll_object_type_t*)LLL_GET_OBJECT_WITH_OFFSET(o,i))=LLL_OBJECT_TYPE_NOP;
					}
					return off+eoff;
				}
				if (*l==1){
					for (uint32_t i=0;i<sizeof(lll_object_t)+sizeof(lll_statement_count_t);i++){
						*((lll_object_type_t*)LLL_GET_OBJECT_WITH_OFFSET(o,i))=LLL_OBJECT_TYPE_NOP;
					}
					return off+eoff;
				}
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_DEBUG_DATA:
			{
				lll_debug_object_t* dbg=(lll_debug_object_t*)o;
				uint32_t sz=sizeof(lll_debug_object_t)+LLL_GET_DEBUG_OBJECT_LINE_NUMBER_WIDTH(dbg)+LLL_GET_DEBUG_OBJECT_COLUMN_NUMBER_WIDTH(dbg)+LLL_GET_DEBUG_OBJECT_FILE_OFFSET_WIDTH(dbg);
				lll_object_t* c=LLL_GET_DEBUG_OBJECT_CHILD(dbg,sz);
				uint8_t ot=LLL_GET_OBJECT_TYPE(c);
				uint32_t off=_optimize_object_internal(c,e);
				if (off==UINT32_MAX){
					return UINT32_MAX;
				}
				off+=sz;
				if (LLL_GET_OBJECT_TYPE(c)!=ot){
					for (uint32_t i=0;i<sz;i++){
						*((lll_object_type_t*)LLL_GET_OBJECT_WITH_OFFSET(o,i))=LLL_OBJECT_TYPE_NOP;
					}
				}
				return off+eoff;
			}
	}
	uint32_t off=sizeof(lll_object_t)+sizeof(lll_arg_count_t);
	lll_arg_count_t* l=LLL_GET_OBJECT_ARGUMENT_COUNT(o);
	lll_arg_count_t cl=*l;
	lll_arg_count_t i=0;
	while (i<cl){
		uint32_t al=_optimize_object_internal(LLL_GET_OBJECT_ARGUMENT(o,off),e);
		if (al==UINT32_MAX){
			return UINT32_MAX;
		}
		lll_object_t* a=LLL_GET_OBJECT_ARGUMENT(o,off);
		off+=al;
		if (LLL_IS_OBJECT_TYPE_MATH_CHAIN(o)){
			while (LLL_GET_OBJECT_TYPE(a)==LLL_OBJECT_TYPE_DEBUG_DATA){
				lll_debug_object_t* dbg=(lll_debug_object_t*)a;
				a=LLL_GET_DEBUG_OBJECT_CHILD(dbg,sizeof(lll_debug_object_t)+LLL_GET_DEBUG_OBJECT_LINE_NUMBER_WIDTH(dbg)+LLL_GET_DEBUG_OBJECT_COLUMN_NUMBER_WIDTH(dbg)+LLL_GET_DEBUG_OBJECT_FILE_OFFSET_WIDTH(dbg));
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
					(*l)--;
					for (uint32_t j=off-al;j<off;j++){
						*((lll_object_type_t*)LLL_GET_OBJECT_WITH_OFFSET(o,j))=LLL_OBJECT_TYPE_NOP;
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
							(*l)--;
							for (uint32_t j=off-al;j<off;j++){
								*((lll_object_type_t*)LLL_GET_OBJECT_WITH_OFFSET(o,j))=LLL_OBJECT_TYPE_NOP;
							}
							break;
						case LLL_OBJECT_TYPE_BIT_NOT:
							printf("SET OBJECT TO -2!\n");
							break;
					}
					break;
				case LLL_OBJECT_TYPE_FALSE:
					switch (LLL_GET_OBJECT_TYPE(o)){
						case LLL_OBJECT_TYPE_ADD:
						case LLL_OBJECT_TYPE_SUB:
						case LLL_OBJECT_TYPE_BIT_OR:
						case LLL_OBJECT_TYPE_BIT_XOR:
							(*l)--;
							for (uint32_t j=off-al;j<off;j++){
								*((lll_object_type_t*)LLL_GET_OBJECT_WITH_OFFSET(o,j))=LLL_OBJECT_TYPE_NOP;
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
							o->t=LLL_OBJECT_TYPE_INT;
							LLL_SET_OBJECT_AS_INT8(o,0);
							for (uint32_t j=sizeof(lll_object_t)+sizeof(int8_t);j<off;j++){
								*((lll_object_type_t*)LLL_GET_OBJECT_WITH_OFFSET(o,j))=LLL_OBJECT_TYPE_NOP;
							}
							return off+eoff;
						case LLL_OBJECT_TYPE_BIT_NOT:
							printf("SET OBJECT TO -1!\n");
							break;
					}
					break;
			}
		}
		i++;
	}
	if (!(*l)&&LLL_IS_OBJECT_TYPE_MATH_CHAIN(o)){
		o->t=LLL_OBJECT_TYPE_INT;
		LLL_SET_OBJECT_AS_INT8(o,0);
		for (uint32_t j=sizeof(lll_object_t)+sizeof(int8_t);j<off;j++){
			*((lll_object_type_t*)LLL_GET_OBJECT_WITH_OFFSET(o,j))=LLL_OBJECT_TYPE_NOP;
		}
		return off+eoff;
	}
	if (*l==1){
		if (LLL_IS_OBJECT_TYPE_COMPARE(o)){
			o->t=LLL_OBJECT_TYPE_TRUE;
			for (uint32_t j=sizeof(lll_object_t);j<off;j++){
				*((lll_object_type_t*)LLL_GET_OBJECT_WITH_OFFSET(o,j))=LLL_OBJECT_TYPE_NOP;
			}
			return off+eoff;
		}
		if (LLL_IS_OBJECT_TYPE_MATH_CHAIN(o)){
			for (uint32_t j=0;j<sizeof(lll_object_t)+sizeof(lll_arg_count_t);j++){
				*((lll_object_type_t*)LLL_GET_OBJECT_WITH_OFFSET(o,j))=LLL_OBJECT_TYPE_NOP;
			}
			return off+eoff;
		}
	}
	return off+eoff;
}



uint32_t _remove_padding_internal(lll_object_t* o,uint32_t* rm){
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
			REPEAT_BYTE_COPY(d,s,sizeof(lll_object_t));
			return sizeof(lll_object_t)+pad;
		case LLL_OBJECT_TYPE_CHAR:
			REPEAT_BYTE_COPY(d,s,sizeof(lll_object_t)+sizeof(char));
			return sizeof(lll_object_t)+sizeof(char)+pad;
		case LLL_OBJECT_TYPE_STRING:
		case LLL_OBJECT_TYPE_IDENTIFIER:;
			lll_string_length_t sl=LLL_GET_OBJECT_STRING_LENGTH(o);
			REPEAT_BYTE_COPY(d,s,sizeof(lll_object_t)+sizeof(lll_string_length_t)+sl);
			return sizeof(lll_object_t)+sizeof(lll_string_length_t)+sl+pad;
		case LLL_OBJECT_TYPE_INT:
			{
				uint32_t w=LLL_GET_OBJECT_INTEGER_WIDTH(o);
				REPEAT_BYTE_COPY(d,s,sizeof(lll_object_t)+w);
				return sizeof(lll_object_t)+w+pad;
			}
		case LLL_OBJECT_TYPE_FLOAT:
			{
				uint32_t w=(LLL_IS_OBJECT_FLOAT64(o)?sizeof(double):sizeof(float));
				REPEAT_BYTE_COPY(d,s,sizeof(lll_object_t)+w);
				return sizeof(lll_object_t)+w+pad;
			}
		case LLL_OBJECT_TYPE_OPERATION_LIST:
			{
				uint32_t off=sizeof(lll_object_t)+sizeof(lll_statement_count_t);
				REPEAT_BYTE_COPY(d,s,sizeof(lll_object_t)+sizeof(lll_statement_count_t));
				lll_statement_count_t l=*LLL_GET_OBJECT_STATEMENT_COUNT(o);
				while (l){
					l--;
					off+=_remove_padding_internal(LLL_GET_OBJECT_STATEMENT(o,off),rm);
				}
				return off+pad;
			}
		case LLL_OBJECT_TYPE_DEBUG_DATA:
			{
				lll_debug_object_t* dbg=(lll_debug_object_t*)o;
				uint32_t sz=sizeof(lll_debug_object_t)+LLL_GET_DEBUG_OBJECT_LINE_NUMBER_WIDTH(dbg)+LLL_GET_DEBUG_OBJECT_COLUMN_NUMBER_WIDTH(dbg)+LLL_GET_DEBUG_OBJECT_FILE_OFFSET_WIDTH(dbg);
				REPEAT_BYTE_COPY(d,s,sz);
				return sz+_remove_padding_internal(LLL_GET_DEBUG_OBJECT_CHILD(dbg,sz),rm)+pad;
			}
	}
	uint32_t off=sizeof(lll_object_t)+sizeof(lll_arg_count_t);
	REPEAT_BYTE_COPY(d,s,sizeof(lll_object_t)+sizeof(lll_arg_count_t));
	lll_arg_count_t l=*LLL_GET_OBJECT_ARGUMENT_COUNT(o);
	while (l){
		l--;
		off+=_remove_padding_internal(LLL_GET_OBJECT_ARGUMENT(o,off),rm);
	}
	return off+pad;
}



__LLL_IMPORT_EXPORT __LLL_CHECK_OUTPUT uint8_t lll_optimize_object(lll_object_t* o,lll_error_t* e){
	if (!_bf){
		e->t=LLL_ERROR_NO_STACK;
		return LLL_RETURN_ERROR;
	}
	if (_optimize_object_internal(o,e)==UINT32_MAX){
		return LLL_RETURN_ERROR;
	}
	return LLL_RETURN_NO_ERROR;
}



__LLL_IMPORT_EXPORT __LLL_CHECK_OUTPUT uint8_t lll_remove_object_padding(lll_object_t* o,lll_error_t* e){
	if (!_bf){
		e->t=LLL_ERROR_NO_STACK;
		return LLL_RETURN_ERROR;
	}
	uint32_t rm=0;
	_remove_padding_internal(o,&rm);
	return LLL_RETURN_NO_ERROR;
}

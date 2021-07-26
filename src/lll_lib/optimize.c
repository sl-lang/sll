#include <lll_lib.h>
#include <_lll_lib_internal.h>
#include <stdint.h>
#include <stdlib.h>



lll_stack_offset_t _optimize_object_internal(lll_object_t* o,lll_error_t* e){
	lll_stack_offset_t eoff=0;
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
			return sizeof(lll_string_object_t)+eoff;
		case LLL_OBJECT_TYPE_IDENTIFIER:
			return sizeof(lll_identifier_object_t)+eoff;
		case LLL_OBJECT_TYPE_INT:
			return sizeof(lll_integer_object_t)+eoff;
		case LLL_OBJECT_TYPE_FLOAT:
			return sizeof(lll_float_object_t)+eoff;
		case LLL_OBJECT_TYPE_FUNC:
			{
				lll_stack_offset_t off=sizeof(lll_function_object_t);
				lll_arg_count_t l=((lll_function_object_t*)o)->ac;
				while (l){
					l--;
					uint32_t aoff=_optimize_object_internal(LLL_GET_OBJECT_ARGUMENT(o,off),e);
					if (!aoff){
						return 0;
					}
					off+=aoff;
				}
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_IMPORT:
			return sizeof(lll_import_object_t)+sizeof(lll_import_index_t)*((lll_import_object_t*)o)->ac+eoff;
		case LLL_OBJECT_TYPE_OPERATION_LIST:
			{
				lll_stack_offset_t off=sizeof(lll_operation_list_object_t);
				lll_statement_count_t l=((lll_operation_list_object_t*)o)->sc;
				for (lll_statement_count_t i=l;i>0;i--){
					lll_object_t* st=LLL_GET_OBJECT_STATEMENT(o,off);
					lll_stack_offset_t st_l=_optimize_object_internal(st,e);
					if (!st_l){
						return 0;
					}
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
						for (lll_stack_offset_t j=off-st_l;j<off;j+=sizeof(lll_object_type_t)){
							LLL_SET_OBJECT_NOP(o,j);
						}
					}
				}
				if (!l){
					o->t=LLL_OBJECT_TYPE_NIL;
					for (lll_stack_offset_t i=sizeof(lll_object_t);i<sizeof(lll_operation_list_object_t);i+=sizeof(lll_object_type_t)){// lgtm [cpp/constant-comparison]
						LLL_SET_OBJECT_NOP(o,i);
					}
					return off+eoff;
				}
				((lll_operation_list_object_t*)o)->sc=l;
				if (l==1){
					for (lll_stack_offset_t i=0;i<sizeof(lll_operation_list_object_t);i+=sizeof(lll_object_type_t)){
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
				lll_stack_offset_t off=_optimize_object_internal(c,e);
				if (!off){
					return 0;
				}
				if (LLL_GET_OBJECT_TYPE(c)!=ot){
					for (lll_stack_offset_t i=0;i<sizeof(lll_debug_object_t);i+=sizeof(lll_object_type_t)){
						LLL_SET_OBJECT_NOP(o,i);
					}
				}
				return sizeof(lll_debug_object_t)+off+eoff;
			}
	}
	lll_stack_offset_t off=sizeof(lll_operator_object_t);
	lll_arg_count_t l=((lll_operator_object_t*)o)->ac;
	lll_arg_count_t cl=l;
	lll_arg_count_t i=0;
	while (i<cl){
		lll_object_t* a=LLL_GET_OBJECT_ARGUMENT(o,off);
		lll_stack_offset_t al=_optimize_object_internal(a,e);
		if (!al){
			return 0;
		}
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
					for (lll_stack_offset_t j=off-al;j<off;j+=sizeof(lll_object_type_t)){
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
							for (lll_stack_offset_t j=off-al;j<off;j+=sizeof(lll_object_type_t)){
								LLL_SET_OBJECT_NOP(o,j);
							}
							break;
						case LLL_OBJECT_TYPE_BIT_NOT:
							ASSERT(!"Set Object To -2!",e,0);
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
							for (lll_stack_offset_t j=off-al;j<off;j+=sizeof(lll_object_type_t)){
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
							return 0;
						case LLL_OBJECT_TYPE_MULT:
						case LLL_OBJECT_TYPE_BIT_AND:
_set_to_0:
							i++;
							while (i<cl){
								i++;
								off+=_get_object_size(LLL_GET_OBJECT_ARGUMENT(o,off));
							}
							ASSERT(!"Set Object to 0!",e,0);
							return off+eoff;
						case LLL_OBJECT_TYPE_BIT_NOT:
							ASSERT(!"Set Object To -1!",e,0);
							break;
					}
					break;
			}
		}
		i++;
	}
	if (!l&&LLL_IS_OBJECT_TYPE_MATH(o)){
		ASSERT(!"Set Object to 0!",e,0);
		return off+eoff;
	}
	((lll_operator_object_t*)o)->ac=l;
	if (l==1){
		if (LLL_IS_OBJECT_TYPE_COMPARE(o)){
			o->t=LLL_OBJECT_TYPE_TRUE;
			for (lll_stack_offset_t j=sizeof(lll_operator_object_t);j<off;j+=sizeof(lll_object_type_t)){
				LLL_SET_OBJECT_NOP(o,j);
			}
			return off+eoff;
		}
		if (LLL_IS_OBJECT_TYPE_MATH(o)){
			for (lll_stack_offset_t j=0;j<sizeof(lll_operator_object_t);j+=sizeof(lll_object_type_t)){
				LLL_SET_OBJECT_NOP(o,j);
			}
			return off+eoff;
		}
	}
	return off+eoff;
}



lll_stack_offset_t _mark_strings_update_imports(const lll_object_t* o,uint64_t* m,lll_import_index_t* im){
	lll_stack_offset_t eoff=0;
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
			*(m+(((lll_string_object_t*)o)->i>>6))|=1ull<<(((lll_string_object_t*)o)->i&63);
			return sizeof(lll_string_object_t)+eoff;
		case LLL_OBJECT_TYPE_IDENTIFIER:
			return sizeof(lll_identifier_object_t)+eoff;
		case LLL_OBJECT_TYPE_INT:
			return sizeof(lll_integer_object_t)+eoff;
		case LLL_OBJECT_TYPE_FLOAT:
			return sizeof(lll_float_object_t)+eoff;
		case LLL_OBJECT_TYPE_FUNC:
			{
				lll_stack_offset_t off=sizeof(lll_function_object_t);
				lll_arg_count_t l=((lll_function_object_t*)o)->ac;
				while (l){
					l--;
					off+=_mark_strings_update_imports(LLL_GET_OBJECT_ARGUMENT(o,off),m,im);
				}
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_IMPORT:
			{
				lll_import_object_t* im_o=(lll_import_object_t*)o;
				for (lll_arg_count_t i=0;i<im_o->ac;i++){
					im_o->idx[i]=*(im+im_o->idx[i]);
				}
				return sizeof(lll_import_object_t)+im_o->ac*sizeof(lll_import_index_t)+eoff;
			}
		case LLL_OBJECT_TYPE_OPERATION_LIST:
			{
				lll_stack_offset_t off=sizeof(lll_operation_list_object_t);
				lll_statement_count_t l=((lll_operation_list_object_t*)o)->sc;
				while (l){
					l--;
					off+=_mark_strings_update_imports(LLL_GET_OBJECT_STATEMENT(o,off),m,im);
				}
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_DEBUG_DATA:
			*(m+(((lll_debug_object_t*)o)->fpi>>6))|=1ull<<(((lll_debug_object_t*)o)->fpi&63);
			return sizeof(lll_debug_object_t)+eoff+_mark_strings_update_imports(LLL_GET_DEBUG_OBJECT_CHILD((lll_debug_object_t*)o),m,im);
	}
	lll_stack_offset_t off=sizeof(lll_operator_object_t);
	lll_arg_count_t l=((lll_operator_object_t*)o)->ac;
	while (l){
		l--;
		off+=_mark_strings_update_imports(LLL_GET_OBJECT_ARGUMENT(o,off),m,im);
	}
	return off+eoff;
}



lll_stack_offset_t _update_strings(const lll_object_t* o,lll_string_index_t* sm){
	lll_stack_offset_t eoff=0;
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
			((lll_string_object_t*)o)->i=*(sm+((lll_string_object_t*)o)->i);
			return sizeof(lll_string_object_t)+eoff;
		case LLL_OBJECT_TYPE_IDENTIFIER:
			return sizeof(lll_identifier_object_t)+eoff;
		case LLL_OBJECT_TYPE_INT:
			return sizeof(lll_integer_object_t)+eoff;
		case LLL_OBJECT_TYPE_FLOAT:
			return sizeof(lll_float_object_t)+eoff;
		case LLL_OBJECT_TYPE_FUNC:
			{
				lll_stack_offset_t off=sizeof(lll_function_object_t);
				lll_arg_count_t l=((lll_function_object_t*)o)->ac;
				while (l){
					l--;
					off+=_update_strings(LLL_GET_OBJECT_ARGUMENT(o,off),sm);
				}
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_IMPORT:
			return sizeof(lll_import_object_t)+sizeof(lll_import_index_t)*((lll_import_object_t*)o)->ac+eoff;
		case LLL_OBJECT_TYPE_OPERATION_LIST:
			{
				lll_stack_offset_t off=sizeof(lll_operation_list_object_t);
				lll_statement_count_t l=((lll_operation_list_object_t*)o)->sc;
				while (l){
					l--;
					off+=_update_strings(LLL_GET_OBJECT_STATEMENT(o,off),sm);
				}
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_DEBUG_DATA:
			((lll_debug_object_t*)o)->fpi=*(sm+((lll_debug_object_t*)o)->fpi);
			return sizeof(lll_debug_object_t)+eoff+_update_strings(LLL_GET_DEBUG_OBJECT_CHILD((lll_debug_object_t*)o),sm);
	}
	lll_stack_offset_t off=sizeof(lll_operator_object_t);
	lll_arg_count_t l=((lll_operator_object_t*)o)->ac;
	while (l){
		l--;
		off+=_update_strings(LLL_GET_OBJECT_ARGUMENT(o,off),sm);
	}
	return off+eoff;
}



lll_stack_offset_t _remove_padding_internal(lll_object_t* o,lll_stack_offset_t* rm,lll_compilation_data_t* c_dt,uint64_t st){
	uint8_t* d=(uint8_t*)o-(*rm);
	lll_stack_offset_t pad=0;
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
			for (lll_stack_offset_t i=0;i<sizeof(lll_object_t);i++){
				*(d+i)=*(s+i);
			}
			return sizeof(lll_object_t)+pad;
		case LLL_OBJECT_TYPE_CHAR:
			for (lll_stack_offset_t i=0;i<sizeof(lll_char_object_t);i++){
				*(d+i)=*(s+i);
			}
			return sizeof(lll_char_object_t)+pad;
		case LLL_OBJECT_TYPE_STRING:
			for (lll_stack_offset_t i=0;i<sizeof(lll_string_object_t);i++){
				*(d+i)=*(s+i);
			}
			return sizeof(lll_string_object_t)+pad;
		case LLL_OBJECT_TYPE_IDENTIFIER:
			for (lll_stack_offset_t i=0;i<sizeof(lll_identifier_object_t);i++){
				*(d+i)=*(s+i);
			}
			return sizeof(lll_identifier_object_t)+pad;
		case LLL_OBJECT_TYPE_INT:
			{
				for (lll_stack_offset_t i=0;i<sizeof(lll_integer_object_t);i++){
					*(d+i)=*(s+i);
				}
				return sizeof(lll_integer_object_t)+pad;
			}
		case LLL_OBJECT_TYPE_FLOAT:
			{
				for (lll_stack_offset_t i=0;i<sizeof(lll_float_object_t);i++){
					*(d+i)=*(s+i);
				}
				return sizeof(lll_float_object_t)+pad;
			}
		case LLL_OBJECT_TYPE_FUNC:
			{
				for (lll_stack_offset_t i=0;i<sizeof(lll_function_object_t);i++){
					*(d+i)=*(s+i);
				}
				(*(c_dt->f_dt.dt+((lll_function_object_t*)o)->id))->off-=(lll_stack_offset_t)(((uint64_t)(void*)s)-((uint64_t)(void*)d));
				lll_stack_offset_t off=sizeof(lll_function_object_t);
				lll_arg_count_t l=((lll_operator_object_t*)o)->ac;
				while (l){
					l--;
					off+=_remove_padding_internal(LLL_GET_OBJECT_ARGUMENT(o,off),rm,c_dt,st);
				}
				return off+pad;
			}
		case LLL_OBJECT_TYPE_IMPORT:
			{
				lll_stack_offset_t sz=sizeof(lll_import_object_t)+sizeof(lll_import_index_t)*((lll_import_object_t*)o)->ac;
				for (lll_stack_offset_t i=0;i<sz;i++){
					*(d+i)=*(s+i);
				}
				return sz+pad;
			}
		case LLL_OBJECT_TYPE_OPERATION_LIST:
			{
				for (lll_stack_offset_t i=0;i<sizeof(lll_operation_list_object_t);i++){
					*(d+i)=*(s+i);
				}
				lll_stack_offset_t off=sizeof(lll_operation_list_object_t);
				lll_statement_count_t l=((lll_operation_list_object_t*)o)->sc;
				while (l){
					l--;
					off+=_remove_padding_internal(LLL_GET_OBJECT_STATEMENT(o,off),rm,c_dt,st);
				}
				return off+pad;
			}
		case LLL_OBJECT_TYPE_DEBUG_DATA:
			for (lll_stack_offset_t i=0;i<sizeof(lll_debug_object_t);i++){
				*(d+i)=*(s+i);
			}
			return sizeof(lll_debug_object_t)+_remove_padding_internal(LLL_GET_DEBUG_OBJECT_CHILD((lll_debug_object_t*)o),rm,c_dt,st)+pad;
	}
	for (lll_stack_offset_t i=0;i<sizeof(lll_operator_object_t);i++){
		*(d+i)=*(s+i);
	}
	lll_stack_offset_t off=sizeof(lll_operator_object_t);
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
	if (!_optimize_object_internal(o,e)){
		return LLL_RETURN_ERROR;
	}
	return LLL_RETURN_NO_ERROR;
}



__LLL_IMPORT_EXPORT __LLL_RETURN lll_optimize_metadata(lll_compilation_data_t* c_dt,lll_error_t* e){
	uint32_t ml=(c_dt->st.l>>6)+1;
	uint64_t* m=malloc(ml*sizeof(uint64_t));
	for (uint32_t i=0;i<ml;i++){
		*(m+i)=0;
	}
	for (uint8_t i=0;i<LLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		lll_identifier_list_t* l=c_dt->i_dt.s+i;
		for (lll_identifier_list_length_t j=0;j<l->l;j++){
			*(m+((l->dt+j)->i>>6))|=1ull<<((l->dt+j)->i&63);
		}
	}
	for (lll_identifier_list_length_t i=0;i<c_dt->i_dt.ill;i++){
		*(m+((c_dt->i_dt.il+i)->i>>6))|=1ull<<((c_dt->i_dt.il+i)->i&63);
	}
	lll_import_index_t* im=malloc(c_dt->im.l*sizeof(lll_import_index_t));
	uint32_t k=0;
	uint32_t l=0;
	for (lll_import_index_t i=0;i<c_dt->im.l;i++){
		lll_string_index_t j=*(c_dt->im.dt+i);
		if (j==LLL_MAX_STRING_INDEX){
			for (uint32_t n=k;n<i;n++){
				*(c_dt->im.dt+n-l)=*(c_dt->im.dt+n);
				*(im+n)=n-l;
			}
			k=i+1;
			l++;
		}
		else{
			*(m+(j>>6))|=1ull<<(j&63);
		}
	}
	for (uint32_t i=k;i<c_dt->im.l;i++){
		*(c_dt->im.dt+i-l)=*(c_dt->im.dt+i);
		*(im+i)=i-l;
	}
	if (l){
		c_dt->im.l-=l;
		c_dt->im.dt=realloc(c_dt->im.dt,c_dt->im.l*sizeof(lll_string_index_t));
	}
	_mark_strings_update_imports(c_dt->h,m,im);
	free(im);
	lll_string_index_t* sm=malloc(c_dt->st.l*sizeof(lll_string_index_t));
	k=0;
	l=0;
	for (uint32_t i=0;i<ml;i++){
		uint64_t v=~(*(m+i));
		while (v){
			lll_string_index_t j=FIND_FIRST_SET_BIT(v)|(i<<6);
			if (j==c_dt->st.l){
				break;
			}
			for (uint32_t n=k;n<j;n++){
				*(c_dt->st.dt+n-l)=*(c_dt->st.dt+n);
				*(sm+n)=n-l;
			}
			k=j+1;
			l++;
			v&=v-1;
		}
	}
	free(m);
	for (uint32_t i=k;i<c_dt->st.l;i++){
		*(c_dt->st.dt+i-l)=*(c_dt->st.dt+i);
		*(sm+i)=i-l;
	}
	if (l){
		c_dt->st.l-=l;
		c_dt->st.dt=realloc(c_dt->st.dt,c_dt->st.l*sizeof(lll_string_t*));
		_update_strings(c_dt->h,sm);
	}
	free(sm);
	return LLL_RETURN_NO_ERROR;
}



__LLL_IMPORT_EXPORT __LLL_RETURN lll_remove_object_padding(lll_compilation_data_t* c_dt,lll_object_t* o,lll_error_t* e){
	if (!_bf){
		e->t=LLL_ERROR_NO_STACK;
		return LLL_RETURN_ERROR;
	}
	lll_stack_offset_t rm=0;
	_remove_padding_internal(o,&rm,c_dt,(uint64_t)(void*)(c_dt->h));
	return LLL_RETURN_NO_ERROR;
}

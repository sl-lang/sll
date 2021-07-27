#include <lll_lib.h>
#include <_lll_lib_internal.h>
#include <stdlib.h>



lll_string_index_t _create_string(lll_compilation_data_t* c_dt,const lll_char_t* dt,lll_string_length_t l){
	lll_string_checksum_t c=0;
	for (lll_string_length_t i=0;i<l;i++){
		c^=(lll_string_checksum_t)(*(dt+i));
	}
	for (lll_string_index_t i=0;i<c_dt->st.l;i++){
		lll_string_t* s=*(c_dt->st.dt+i);
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
	c_dt->st.l++;
	c_dt->st.dt=realloc(c_dt->st.dt,c_dt->st.l*sizeof(lll_string_t*));
	lll_string_t* s=malloc(sizeof(lll_string_t)+(l+1)*sizeof(lll_char_t));
	s->l=l;
	s->c=c;
	for (lll_string_length_t i=0;i<l;i++){
		s->v[i]=*(dt+i);
	}
	s->v[l]=0;
	*(c_dt->st.dt+c_dt->st.l-1)=s;
	return c_dt->st.l-1;
}



lll_stack_offset_t _get_object_size(const lll_object_t* o){
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
					off+=_get_object_size(LLL_GET_OBJECT_ARGUMENT(o,off));
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
					off+=_get_object_size(LLL_GET_OBJECT_STATEMENT(o,off));
				}
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_DEBUG_DATA:
			return sizeof(lll_debug_object_t)+eoff+_get_object_size(LLL_GET_DEBUG_OBJECT_CHILD((lll_debug_object_t*)o));
	}
	lll_stack_offset_t off=sizeof(lll_operator_object_t);
	lll_arg_count_t l=((lll_operator_object_t*)o)->ac;
	while (l){
		l--;
		off+=_get_object_size(LLL_GET_OBJECT_ARGUMENT(o,off));
	}
	return off+eoff;
}

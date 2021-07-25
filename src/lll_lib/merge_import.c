#include <lll_lib.h>
#include <_lll_lib_internal.h>
#include <stdint.h>
#include <stdlib.h>



lll_stack_offset_t _patch_import(lll_object_t* o,import_data_t* dt){
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
		case LLL_OBJECT_TYPE_IMPORT:
			{
				lll_import_object_t* io=(lll_import_object_t*)o;
				lll_stack_offset_t sz=sizeof(lll_import_object_t)+sizeof(lll_import_index_t)*io->ac;
				lll_arg_count_t i=io->ac;
				while (i){
					i--;
					if (io->idx[i]==dt->i){
						for (lll_arg_count_t j=i+1;j<io->ac;j++){
							io->idx[j-1]=io->idx[j];
						}
						io->ac--;
					}
				}
				lll_stack_offset_t j=(!io->ac?0:sizeof(lll_import_object_t)+sizeof(lll_import_index_t)*io->ac);
				if (j!=sz){
					if (dt->off==LLL_MAX_STACK_OFFSET){
						dt->off=(lll_stack_offset_t)(((uint64_t)(void*)o)-dt->b_off);
					}
					else{
						dt->rm=1;
					}
				}
				while (j<sz){
					LLL_SET_OBJECT_NOP(o,j);
					j+=sizeof(lll_object_type_t);
				}
				return sz+eoff;
			}
		case LLL_OBJECT_TYPE_FUNC:
			{
				lll_stack_offset_t off=sizeof(lll_function_object_t);
				lll_arg_count_t l=((lll_function_object_t*)o)->ac;
				lll_arg_count_t sz=l;
				while (l){
					l--;
					off+=_patch_import(LLL_GET_OBJECT_ARGUMENT(o,off),dt);
					if (dt->rm){
						dt->rm=0;
						sz--;
					}
				}
				((lll_function_object_t*)o)->ac=sz;
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_OPERATION_LIST:
			{
				lll_stack_offset_t off=sizeof(lll_operation_list_object_t);
				lll_statement_count_t l=((lll_operation_list_object_t*)o)->sc;
				lll_statement_count_t sz=l;
				while (l){
					l--;
					off+=_patch_import(LLL_GET_OBJECT_STATEMENT(o,off),dt);
					if (dt->rm){
						dt->rm=0;
						sz--;
					}
				}
				((lll_operation_list_object_t*)o)->sc=sz;
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_DEBUG_DATA:
			{
				lll_debug_object_t* dbg=(lll_debug_object_t*)o;
				return sizeof(lll_debug_object_t)+eoff+_patch_import(LLL_GET_DEBUG_OBJECT_CHILD(dbg),dt);
			}
	}
	lll_stack_offset_t off=sizeof(lll_operator_object_t);
	lll_arg_count_t l=((lll_operator_object_t*)o)->ac;
	lll_arg_count_t sz=l;
	while (l){
		l--;
		off+=_patch_import(LLL_GET_OBJECT_ARGUMENT(o,off),dt);
		if (dt->rm){
			dt->rm=0;
			sz--;
		}
	}
	((lll_operator_object_t*)o)->ac=sz;
	return off+eoff;
}



lll_stack_offset_t _patch_module(lll_object_t* o,const import_module_data_t* im_dt){
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
			((lll_string_object_t*)o)->i=*(im_dt->sm+((lll_string_object_t*)o)->i);
			return sizeof(lll_string_object_t)+eoff;
		case LLL_OBJECT_TYPE_IDENTIFIER:
			{
				lll_identifier_object_t* io=(lll_identifier_object_t*)o;
				io->idx=LLL_IDENTIFIER_ADD_INDEX(io->idx,im_dt->off[LLL_IDENTIFIER_GET_ARRAY_ID(io->idx)]);
				return sizeof(lll_identifier_object_t)+eoff;
			}
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
					off+=_patch_module(LLL_GET_OBJECT_ARGUMENT(o,off),im_dt);
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
					off+=_patch_module(LLL_GET_OBJECT_STATEMENT(o,off),im_dt);
				}
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_DEBUG_DATA:
			{
				lll_debug_object_t* dbg=(lll_debug_object_t*)o;
				dbg->fpi+=im_dt->dbg_off;
				return sizeof(lll_debug_object_t)+eoff+_patch_module(LLL_GET_DEBUG_OBJECT_CHILD(dbg),im_dt);
			}
	}
	lll_stack_offset_t off=sizeof(lll_operator_object_t);
	lll_arg_count_t l=((lll_operator_object_t*)o)->ac;
	while (l){
		l--;
		off+=_patch_module(LLL_GET_OBJECT_ARGUMENT(o,off),im_dt);
	}
	return off+eoff;
}



__LLL_IMPORT_EXPORT __LLL_RETURN lll_merge_import(lll_compilation_data_t* c_dt,lll_import_index_t im_i,lll_compilation_data_t* im,lll_error_t* e){
	if (im_i>=c_dt->im.l||*(c_dt->im.dt+im_i)==LLL_MAX_STRING_INDEX){
		e->t=LLL_ERROR_INVALID_IMPORT_INDEX;
		e->dt.im_i=im_i;
		return LLL_RETURN_ERROR;
	}
	*(c_dt->im.dt+im_i)=LLL_MAX_STRING_INDEX;
	import_data_t dt={
		im_i,
		LLL_MAX_STACK_OFFSET,
		(uint64_t)(void*)(c_dt->h),
		0
	};
	lll_stack_offset_t sz=_patch_import(c_dt->h,&dt);
	import_module_data_t im_dt={
		.dbg_off=c_dt->fp_dt.l,
		.sm=malloc(im->st.l*sizeof(lll_string_index_t))
	};
	for (lll_string_index_t i=0;i<im->st.l;i++){
		lll_string_t* s=*(im->st.dt+i);
		for (lll_string_index_t j=0;j<c_dt->st.l;j++){
			lll_string_t* d=*(c_dt->st.dt+j);
			if (s->c!=d->c||s->l!=d->l){
				continue;
			}
			for (lll_string_length_t k=0;k<s->l;k++){
				if (s->v[k]!=d->v[k]){
					goto _check_next_string;
				}
			}
			*(im_dt.sm+i)=j;
			goto _merge_next_string;
_check_next_string:;
		}
		*(im_dt.sm+i)=c_dt->st.l;
		c_dt->st.l++;
		c_dt->st.dt=realloc(c_dt->st.dt,c_dt->st.l*sizeof(lll_string_t*));
		lll_string_t* n=malloc(sizeof(lll_string_t)+(s->l+1)*sizeof(char));
		n->l=s->l;
		n->c=s->c;
		for (lll_string_length_t j=0;j<s->l;j++){
			n->v[j]=s->v[j];
		}
		n->v[n->l]=0;
		*(c_dt->st.dt+c_dt->st.l-1)=n;
_merge_next_string:;
	}
	lll_file_path_index_t si=c_dt->fp_dt.l;
	c_dt->fp_dt.l+=im->fp_dt.l;
	void* tmp=realloc(c_dt->fp_dt.dt,c_dt->fp_dt.l*sizeof(lll_string_index_t));
	if (!tmp){
		ASSERT(!"Unable to Reallocate File Path Array",e,LLL_RETURN_ERROR);
	}
	c_dt->fp_dt.dt=tmp;
	for (lll_file_path_index_t i=0;i<im->fp_dt.l;i++){
		*(c_dt->fp_dt.dt+si+i)=*(im_dt.sm+*(im->fp_dt.dt+i));
	}
	for (uint8_t i=0;i<LLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		lll_identifier_list_t* il=c_dt->i_dt.s+i;
		lll_identifier_list_t mil=im->i_dt.s[i];
		im_dt.off[i]=il->l;
		if (mil.l){
			si=il->l;
			il->l+=mil.l;
			tmp=realloc(il->dt,il->l*sizeof(lll_identifier_t));
			if (!tmp){
				ASSERT(!"Unable to Reallocate Fixed-Length Identifier Array",e,LLL_RETURN_ERROR);
			}
			il->dt=tmp;
			for (lll_identifier_list_length_t j=0;j<mil.l;j++){
				(il->dt+si+j)->sc=(mil.dt+j)->sc+c_dt->_n_sc_id;
				(il->dt+si+j)->i=*(im_dt.sm+(mil.dt+j)->i);
			}
		}
	}
	im_dt.off[LLL_MAX_SHORT_IDENTIFIER_LENGTH]=c_dt->i_dt.ill;
	if (im->i_dt.ill){
		si=c_dt->i_dt.ill;
		c_dt->i_dt.ill+=im->i_dt.ill;
		tmp=realloc(c_dt->i_dt.il,c_dt->i_dt.ill*sizeof(lll_identifier_t));
		if (!tmp){
			ASSERT(!"Unable to Reallocate Variable-Length Identifier Array",e,LLL_RETURN_ERROR);
		}
		c_dt->i_dt.il=tmp;
		for (lll_identifier_list_length_t j=0;j<im->i_dt.ill;j++){
			(c_dt->i_dt.il+si+j)->sc=(im->i_dt.il+j)->sc+c_dt->_n_sc_id;
			(c_dt->i_dt.il+si+j)->i=*(im_dt.sm+(im->i_dt.il+j)->i);
		}
	}
	c_dt->_n_sc_id+=im->_n_sc_id;
	lll_stack_offset_t m_sz=_patch_module(im->h,&im_dt);
	free(im_dt.sm);
	if (sz+m_sz>=_bf_sz){
		e->t=LLL_ERROR_INTERNAL_STACK_OVERFLOW;
		e->dt.r.off=0;
		e->dt.r.sz=1;
		return LLL_RETURN_ERROR;
	}
	sz-=dt.off;
	uint8_t* s=(uint8_t*)(void*)(((uint64_t)(void*)c_dt->h)+dt.off+sz);
	uint8_t* d=(uint8_t*)(void*)(((uint64_t)(void*)c_dt->h)+dt.off+sz+m_sz);
	while (sz){
		sz--;
		s--;
		d--;
		*d=*s;
	}
	s=(uint8_t*)(void*)(((uint64_t)(void*)im->h)+m_sz);
	d=(uint8_t*)(void*)(((uint64_t)(void*)c_dt->h)+dt.off+m_sz);
	while (m_sz){
		m_sz--;
		s--;
		d--;
		*d=*s;
	}
	return LLL_RETURN_NO_ERROR;
}

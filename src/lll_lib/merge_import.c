#include <lll_lib.h>
#include <_lll_internal.h>
#include <stdint.h>
#include <stdlib.h>



uint32_t _patch_import(lll_object_t* o,import_data_t* dt){
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
		case LLL_OBJECT_TYPE_IMPORT:
			{
				lll_import_object_t* io=(lll_import_object_t*)o;
				uint32_t sz=sizeof(lll_import_object_t)+sizeof(lll_import_index_t)*io->ac;
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
				uint32_t j=(!io->ac?0:sizeof(lll_import_object_t)+sizeof(lll_import_index_t)*io->ac);
				if (j!=sz){
					if (dt->off==UINT32_MAX){
						dt->off=(uint32_t)(((uint64_t)(void*)o)-dt->b_off);
					}
					else{
						dt->rm=1;
					}
				}
				while (j<sz){
					LLL_SET_OBJECT_NOP(o,j);
					j++;
				}
				return sz+eoff;
			}
		case LLL_OBJECT_TYPE_FUNC:
			{
				uint32_t off=sizeof(lll_function_object_t);
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
				uint32_t off=sizeof(lll_operation_list_object_t);
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
	uint32_t off=sizeof(lll_operator_object_t);
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



uint32_t _patch_module(lll_object_t* o,import_identifier_offset_list_t* i_off){
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
			{
				lll_identifier_object_t* io=(lll_identifier_object_t*)o;
				io->idx=LLL_IDENTIFIER_ADD_INDEX(io->idx,i_off->off[LLL_IDENTIFIER_GET_ARRAY_ID(io->idx)]);
				return sizeof(lll_identifier_object_t)+eoff;
			}
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
					off+=_patch_module(LLL_GET_OBJECT_ARGUMENT(o,off),i_off);
				}
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_IMPORT:
			return sizeof(lll_import_object_t)+sizeof(lll_import_index_t)*((lll_import_object_t*)o)->ac+eoff;
		case LLL_OBJECT_TYPE_OPERATION_LIST:
			{
				uint32_t off=sizeof(lll_operation_list_object_t);
				lll_statement_count_t l=((lll_operation_list_object_t*)o)->sc;
				while (l){
					l--;
					off+=_patch_module(LLL_GET_OBJECT_STATEMENT(o,off),i_off);
				}
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_DEBUG_DATA:
			{
				lll_debug_object_t* dbg=(lll_debug_object_t*)o;
				dbg->fpi+=i_off->dbg_off;
				return sizeof(lll_debug_object_t)+eoff+_patch_module(LLL_GET_DEBUG_OBJECT_CHILD(dbg),i_off);
			}
	}
	uint32_t off=sizeof(lll_operator_object_t);
	lll_arg_count_t l=((lll_operator_object_t*)o)->ac;
	while (l){
		l--;
		off+=_patch_module(LLL_GET_OBJECT_ARGUMENT(o,off),i_off);
	}
	return off+eoff;
}



__LLL_IMPORT_EXPORT __LLL_RETURN lll_merge_import(lll_compilation_data_t* c_dt,uint32_t im_i,lll_compilation_data_t* im,lll_error_t* e){
	if (im_i>=c_dt->im.l||(c_dt->im.dt+im_i)->sz==UINT32_MAX){
		e->t=LLL_ERROR_INVALID_IMPORT_INDEX;
		e->dt.im_i=im_i;
		return LLL_RETURN_ERROR;
	}
	free((c_dt->im.dt+im_i)->nm);
	(c_dt->im.dt+im_i)->nm=NULL;
	(c_dt->im.dt+im_i)->sz=UINT32_MAX;
	import_data_t dt={
		im_i,
		UINT32_MAX,
		(uint64_t)(void*)(c_dt->h),
		0
	};
	uint32_t sz=_patch_import(c_dt->h,&dt);
	import_identifier_offset_list_t i_off={
		.dbg_off=c_dt->fp_dt.l
	};
	uint32_t si=c_dt->fp_dt.l;
	c_dt->fp_dt.l+=im->fp_dt.l;
	void* tmp=realloc(c_dt->fp_dt.dt,c_dt->fp_dt.l*sizeof(lll_file_path_t));
	if (!tmp){
		ASSERT(!"Unable to Reallocate File Path Array",e,LLL_RETURN_ERROR);
	}
	c_dt->fp_dt.dt=tmp;
	for (uint16_t i=0;i<im->fp_dt.l;i++){
		lll_file_path_t* ifp=c_dt->fp_dt.dt+si+i;
		lll_file_path_t* iifp=im->fp_dt.dt+i;
		ifp->l=iifp->l;
		for (uint16_t j=0;j<ifp->l+1;j++){
			ifp->fp[j]=iifp->fp[j];
		}
	}
	for (uint8_t i=0;i<LLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		lll_identifier_list_t* il=c_dt->i_dt.s+i;
		lll_identifier_list_t mil=im->i_dt.s[i];
		i_off.off[i]=il->l;
		if (mil.l){
			si=il->l;
			il->l+=mil.l;
			tmp=realloc(il->dt,il->l*sizeof(lll_small_identifier_t));
			if (!tmp){
				ASSERT(!"Unable to Reallocate Short Identifier Array",e,LLL_RETURN_ERROR);
			}
			il->dt=tmp;
			for (uint32_t j=0;j<mil.l;j++){
				(il->dt+si+j)->v=malloc((i+1)*sizeof(char));
				(il->dt+si+j)->sc=(mil.dt+j)->sc+c_dt->_n_sc_id;
				for (uint8_t k=0;k<i+1;k++){
					*((il->dt+si+j)->v+k)=*((mil.dt+j)->v+k);
				}
			}
		}
	}
	i_off.off[LLL_MAX_SHORT_IDENTIFIER_LENGTH]=c_dt->i_dt.ill;
	if (im->i_dt.ill){
		si=c_dt->i_dt.ill;
		c_dt->i_dt.ill+=im->i_dt.ill;
		tmp=realloc(c_dt->i_dt.il,c_dt->i_dt.ill*sizeof(lll_identifier_t*));
		if (!tmp){
			ASSERT(!"Unable to Reallocate Long Identifier Array",e,LLL_RETURN_ERROR);
		}
		c_dt->i_dt.il=tmp;
		for (uint32_t j=0;j<im->i_dt.ill;j++){
			lll_identifier_t* mid=*(im->i_dt.il+j);
			lll_identifier_t* id=malloc(sizeof(lll_identifier_t)+mid->sz*sizeof(char));
			id->sz=mid->sz;
			id->sc=mid->sc+c_dt->_n_sc_id;
			for (uint32_t k=0;k<id->sz;k++){
				id->v[k]=mid->v[k];
			}
			*(c_dt->i_dt.il+si+j)=id;
		}
	}
	c_dt->_n_sc_id+=im->_n_sc_id;
	uint32_t m_sz=_patch_module(im->h,&i_off);
	if (sz+m_sz>=_bf_sz){
		e->t=LLL_ERROR_INTERNAL_STACK_OVERFLOW;
		e->dt.r.off=0;
		e->dt.r.sz=1;
		return LLL_RETURN_ERROR;
	}
	sz-=dt.off;
	uint8_t* s=(uint8_t*)LLL_GET_OBJECT_WITH_OFFSET(c_dt->h,dt.off+sz);
	uint8_t* d=(uint8_t*)LLL_GET_OBJECT_WITH_OFFSET(c_dt->h,dt.off+sz+m_sz);
	while (sz){
		sz--;
		s--;
		d--;
		*d=*s;
	}
	s=(uint8_t*)LLL_GET_OBJECT_WITH_OFFSET(im->h,m_sz);
	d=(uint8_t*)LLL_GET_OBJECT_WITH_OFFSET(c_dt->h,dt.off+m_sz);
	while (m_sz){
		m_sz--;
		s--;
		d--;
		*d=*s;
	}
	return LLL_RETURN_NO_ERROR;
}

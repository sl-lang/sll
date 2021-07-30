#include <lll_lib.h>
#include <_lll_lib_internal.h>
#include <stdint.h>
#include <stdlib.h>



lll_object_offset_t _patch_import(lll_object_t* o,import_data_t* dt){
	lll_object_offset_t eoff=0;
	while (o->t==LLL_OBJECT_TYPE_NOP||o->t==LLL_OBJECT_TYPE_DEBUG_DATA){
		eoff++;
		o++;
	}
	switch (o->t){
		case LLL_OBJECT_TYPE_UNKNOWN:
		case LLL_OBJECT_TYPE_CHAR:
		case LLL_OBJECT_TYPE_STRING:
		case LLL_OBJECT_TYPE_IDENTIFIER:
		case LLL_OBJECT_TYPE_INT:
		case LLL_OBJECT_TYPE_FLOAT:
			return eoff+1;
		case LLL_OBJECT_TYPE_IMPORT:
			{
				if (o->dt.ii==dt->i){
					o->t=LLL_OBJECT_TYPE_NOP;
					if (dt->off==LLL_MAX_OBJECT_OFFSET){
						dt->off=(lll_object_offset_t)((((uint64_t)(void*)o)-dt->b_off)/sizeof(lll_object_t));
					}
					else{
						dt->rm=1;
					}
				}
				return eoff+1;
			}
		case LLL_OBJECT_TYPE_FUNC:
			{
				lll_object_offset_t off=1;
				lll_arg_count_t l=o->dt.fn.ac;
				while (l){
					l--;
					off+=_patch_import(o+off,dt);
					if (dt->rm){
						dt->rm=0;
						o->dt.fn.ac--;
					}
				}
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_OPERATION_LIST:
			{
				lll_object_offset_t off=1;
				lll_statement_count_t l=o->dt.sc;
				while (l){
					l--;
					off+=_patch_import(o+off,dt);
					if (dt->rm){
						dt->rm=0;
						o->dt.sc--;
					}
				}
				return off+eoff;
			}
	}
	lll_object_offset_t off=1;
	lll_arg_count_t l=o->dt.ac;
	while (l){
		l--;
		off+=_patch_import(o+off,dt);
		if (dt->rm){
			dt->rm=0;
			o->dt.ac--;
		}
	}
	return off+eoff;
}



lll_object_offset_t _patch_module(lll_object_t* o,const import_module_data_t* im_dt){
	lll_object_offset_t eoff=0;
	while (o->t==LLL_OBJECT_TYPE_NOP){
		eoff++;
		o++;
	}
	switch (o->t){
		case LLL_OBJECT_TYPE_UNKNOWN:
		case LLL_OBJECT_TYPE_CHAR:
		case LLL_OBJECT_TYPE_INT:
		case LLL_OBJECT_TYPE_FLOAT:
		case LLL_OBJECT_TYPE_IMPORT:
			return eoff+1;
		case LLL_OBJECT_TYPE_STRING:
			o->dt.s=*(im_dt->sm+o->dt.s);
			return eoff+1;
		case LLL_OBJECT_TYPE_IDENTIFIER:
			o->dt.id=LLL_IDENTIFIER_ADD_INDEX(o->dt.id,im_dt->off[LLL_IDENTIFIER_GET_ARRAY_ID(o->dt.id)]);
			return eoff+1;
		case LLL_OBJECT_TYPE_FUNC:
			{
				lll_object_offset_t off=1;
				lll_arg_count_t l=o->dt.fn.ac;
				while (l){
					l--;
					off+=_patch_module(o+off,im_dt);
				}
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_OPERATION_LIST:
			{
				lll_object_offset_t off=1;
				lll_statement_count_t l=o->dt.sc;
				while (l){
					l--;
					off+=_patch_module(o+off,im_dt);
				}
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_DEBUG_DATA:
			o->dt.dbg.fpi=*(im_dt->sm+o->dt.dbg.fpi);
			return eoff+_patch_module(o+1,im_dt)+1;
	}
	lll_object_offset_t off=1;
	lll_arg_count_t l=o->dt.ac;
	while (l){
		l--;
		off+=_patch_module(o+off,im_dt);
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
		LLL_MAX_OBJECT_OFFSET,
		(uint64_t)(void*)(c_dt->h),
		0
	};
	lll_object_offset_t sz=_patch_import(c_dt->h,&dt);
	import_module_data_t im_dt={
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
		lll_string_t* n=malloc(sizeof(lll_string_t)+(s->l+1)*sizeof(lll_char_t));
		n->l=s->l;
		n->c=s->c;
		for (lll_string_length_t j=0;j<s->l;j++){
			n->v[j]=s->v[j];
		}
		n->v[n->l]=0;
		*(c_dt->st.dt+c_dt->st.l-1)=n;
_merge_next_string:;
	}
	for (uint8_t i=0;i<LLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		lll_identifier_list_t* il=c_dt->i_dt.s+i;
		lll_identifier_list_t mil=im->i_dt.s[i];
		im_dt.off[i]=il->l;
		if (mil.l){
			lll_identifier_list_length_t si=il->l;
			il->l+=mil.l;
			void* tmp=realloc(il->dt,il->l*sizeof(lll_identifier_t));
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
		lll_identifier_list_length_t si=c_dt->i_dt.ill;
		c_dt->i_dt.ill+=im->i_dt.ill;
		void* tmp=realloc(c_dt->i_dt.il,c_dt->i_dt.ill*sizeof(lll_identifier_t));
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
	lll_object_offset_t m_sz=_patch_module(im->h,&im_dt);
	free(im_dt.sm);
	if (sz+m_sz>=c_dt->_s.sz){
		e->t=LLL_ERROR_INTERNAL_STACK_OVERFLOW;
		e->dt.r.off=0;
		e->dt.r.sz=1;
		return LLL_RETURN_ERROR;
	}
	sz-=dt.off;
	lll_object_t* s=c_dt->h+dt.off+sz;
	lll_object_t* d=c_dt->h+dt.off+sz+m_sz;
	while (sz){
		sz--;
		s--;
		d--;
		*d=*s;
	}
	s=im->h+m_sz;
	d=c_dt->h+dt.off+m_sz;
	while (m_sz){
		m_sz--;
		s--;
		d--;
		*d=*s;
	}
	return LLL_RETURN_NO_ERROR;
}

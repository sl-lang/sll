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
				if (o->dt.im.ii==dt->i){
					if (dt->off==LLL_MAX_OBJECT_OFFSET){
						dt->off=(lll_object_offset_t)(o-dt->b_off);
					}
					else{
						dt->rm=1;
					}
				}
				return eoff+1;
			}
		case LLL_OBJECT_TYPE_FUNC:
		case LLL_OBJECT_TYPE_INTERNAL_FUNC:
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
		case LLL_OBJECT_TYPE_INTERNAL_FUNC:
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
	if (im_i>=c_dt->it.l||*(c_dt->it.dt+im_i)==LLL_MAX_STRING_INDEX){
		e->t=LLL_ERROR_INVALID_IMPORT_INDEX;
		e->dt.im_i=im_i;
		return LLL_RETURN_ERROR;
	}
	*(c_dt->it.dt+im_i)=LLL_MAX_STRING_INDEX;
	import_data_t dt={
		im_i,
		LLL_MAX_OBJECT_OFFSET,
		c_dt->h,
		0
	};
	lll_object_offset_t sz=_patch_import(c_dt->h,&dt);
	ASSERT((c_dt->h+dt.off)->t==LLL_OBJECT_TYPE_IMPORT);
	(c_dt->h+dt.off)->t=LLL_OBJECT_TYPE_NOP;
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
	lll_identifier_list_length_t s_si[LLL_MAX_SHORT_IDENTIFIER_LENGTH];
	for (uint8_t i=0;i<LLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		lll_identifier_list_t* il=c_dt->idt.s+i;
		lll_identifier_list_t mil=im->idt.s[i];
		im_dt.off[i]=il->l;
		if (mil.l){
			s_si[i]=il->l;
			il->l+=mil.l;
			void* tmp=realloc(il->dt,il->l*sizeof(lll_identifier_t));
			if (!tmp){
				ASSERT(!"Unable to Reallocate Fixed-Length Identifier Array",e,LLL_RETURN_ERROR);
			}
			il->dt=tmp;
			for (lll_identifier_list_length_t j=0;j<mil.l;j++){
				(il->dt+s_si[i]+j)->sc=(mil.dt+j)->sc+c_dt->_n_sc_id;
				(il->dt+s_si[i]+j)->i=*(im_dt.sm+(mil.dt+j)->i);
			}
		}
	}
	im_dt.off[LLL_MAX_SHORT_IDENTIFIER_LENGTH]=c_dt->idt.ill;
	lll_identifier_list_length_t si=c_dt->idt.ill;
	if (im->idt.ill){
		c_dt->idt.ill+=im->idt.ill;
		void* tmp=realloc(c_dt->idt.il,c_dt->idt.ill*sizeof(lll_identifier_t));
		if (!tmp){
			ASSERT(!"Unable to Reallocate Variable-Length Identifier Array",e,LLL_RETURN_ERROR);
		}
		c_dt->idt.il=tmp;
		for (lll_identifier_list_length_t j=0;j<im->idt.ill;j++){
			(c_dt->idt.il+si+j)->sc=(im->idt.il+j)->sc+c_dt->_n_sc_id;
			(c_dt->idt.il+si+j)->i=*(im_dt.sm+(im->idt.il+j)->i);
		}
	}
	for (lll_export_table_length_t i=0;i<im->et.l;i++){
		lll_identifier_index_t ei=*(im->et.dt+i);
		uint8_t j=LLL_IDENTIFIER_GET_ARRAY_ID(ei);
		if (j==LLL_MAX_SHORT_IDENTIFIER_LENGTH){
			(c_dt->idt.il+si+LLL_IDENTIFIER_GET_ARRAY_INDEX(ei))->sc=(c_dt->h+dt.off)->dt.im.sc;
		}
		else{
			(c_dt->idt.s[j].dt+s_si[j]+LLL_IDENTIFIER_GET_ARRAY_INDEX(ei))->sc=(c_dt->h+dt.off)->dt.im.sc;
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
		if (s->t==LLL_OBJECT_TYPE_FUNC){
			(*(c_dt->ft.dt+s->dt.fn.id))->off+=m_sz;
		}
		*d=*s;
	}
	s=im->h+m_sz;
	d=c_dt->h+dt.off+m_sz;
	while (m_sz){
		m_sz--;
		s--;
		d--;
		if (s->t==LLL_OBJECT_TYPE_FUNC){
			s->dt.fn.id+=c_dt->ft.l;
		}
		*d=*s;
	}
	lll_function_index_t j=c_dt->ft.l;
	c_dt->ft.l+=im->ft.l;
	c_dt->ft.dt=realloc(c_dt->ft.dt,c_dt->ft.l*sizeof(lll_function_t*));
	for (lll_function_index_t i=0;i<im->ft.l;i++){
		lll_function_t* f=*(im->ft.dt+i);
		lll_function_t* nf=malloc(sizeof(lll_function_t)+f->al*sizeof(lll_identifier_index_t));
		nf->off=f->off+dt.off;
		nf->al=f->al;
		for (lll_arg_count_t k=0;k<f->al;k++){
			nf->a[k]=f->a[k];
		}
		*(c_dt->ft.dt+i+j)=nf;
	}
	return LLL_RETURN_NO_ERROR;
}

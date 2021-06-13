#include <lll_lib.h>
#include <_lll_internal.h>
#include <stdint.h>



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
			return sizeof(lll_object_t)+eoff+sizeof(char);
		case LLL_OBJECT_TYPE_STRING:
			return sizeof(lll_object_t)+eoff+sizeof(lll_string_length_t)+LLL_GET_OBJECT_STRING_LENGTH(o);
		case LLL_OBJECT_TYPE_IDENTIFIER:
			return sizeof(lll_object_t)+eoff+sizeof(lll_identifier_index_t);
		case LLL_OBJECT_TYPE_INT:
			return sizeof(lll_object_t)+eoff+LLL_GET_OBJECT_INTEGER_WIDTH(o);
		case LLL_OBJECT_TYPE_FLOAT:
			return sizeof(lll_object_t)+eoff+(LLL_IS_OBJECT_FLOAT64(o)?sizeof(double):sizeof(float));
		case LLL_OBJECT_TYPE_IMPORT:
			{
				lll_arg_count_t l=*LLL_GET_OBJECT_ARGUMENT_COUNT(o);
				lll_arg_count_t i=l;
				uint32_t sz=sizeof(lll_object_t)+sizeof(lll_arg_count_t)+sizeof(lll_import_index_t)*l;
				while (i){
					i--;
					if (LLL_GET_OBJECT_IMPORT_INDEX(o,i)==dt->i){
						for (lll_arg_count_t j=i+1;j<l;j++){
							LLL_SET_OBJECT_IMPORT_INDEX(o,j-1,LLL_GET_OBJECT_IMPORT_INDEX(o,j));
						}
						l--;
					}
				}
				uint32_t j=(!l?0:sizeof(lll_object_t)+sizeof(lll_arg_count_t)+sizeof(lll_import_index_t)*l);
				if (j!=sz){
					*LLL_GET_OBJECT_ARGUMENT_COUNT(o)=l;
					if (dt->off==UINT32_MAX){
						dt->off=(uint32_t)(((uint64_t)(void*)o)-dt->b_off);
					}
					else{
						dt->rm=1;
					}
				}
				while (j<sz){
					*((lll_object_type_t*)LLL_GET_OBJECT_WITH_OFFSET(o,j))=LLL_OBJECT_TYPE_NOP;
					j++;
				}
				return sz+eoff;
			}
		case LLL_OBJECT_TYPE_OPERATION_LIST:
			{
				uint32_t off=sizeof(lll_object_t)+sizeof(lll_statement_count_t);
				lll_statement_count_t l=*LLL_GET_OBJECT_STATEMENT_COUNT(o);
				lll_statement_count_t sz=l;
				while (l){
					l--;
					off+=_patch_import(LLL_GET_OBJECT_STATEMENT(o,off),dt);
					if (dt->rm){
						dt->rm=0;
						sz--;
					}
				}
				*LLL_GET_OBJECT_STATEMENT_COUNT(o)=sz;
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_DEBUG_DATA:
			{
				lll_debug_object_t* dbg=(lll_debug_object_t*)o;
				uint32_t sz=sizeof(lll_debug_object_t)+LLL_GET_DEBUG_OBJECT_LINE_NUMBER_WIDTH(dbg)+LLL_GET_DEBUG_OBJECT_COLUMN_NUMBER_WIDTH(dbg)+LLL_GET_DEBUG_OBJECT_FILE_OFFSET_WIDTH(dbg);
				return sz+eoff+_patch_import(LLL_GET_DEBUG_OBJECT_CHILD(dbg,sz),dt);
			}
	}
	uint32_t off=sizeof(lll_object_t)+sizeof(lll_arg_count_t);
	lll_arg_count_t l=*LLL_GET_OBJECT_ARGUMENT_COUNT(o);
	lll_arg_count_t sz=l;
	while (l){
		l--;
		off+=_patch_import(LLL_GET_OBJECT_ARGUMENT(o,off),dt);
		if (dt->rm){
			dt->rm=0;
			sz--;
		}
	}
	*LLL_GET_OBJECT_ARGUMENT_COUNT(o)=sz;
	return off+eoff;
}



__LLL_IMPORT_EXPORT __LLL_CHECK_OUTPUT uint8_t lll_merge_import(lll_compilation_data_t* c_dt,uint32_t im_i,lll_compilation_data_t* im,lll_error_t* e){
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
	uint32_t m_sz=_get_object_size(im->h);
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

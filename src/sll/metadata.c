#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/core.h>
#include <sll/gc.h>
#include <sll/types.h>
#include <stdint.h>
#include <stdlib.h>



sll_object_offset_t _mark_strings(sll_object_t* o,uint64_t* m){
	sll_object_offset_t eoff=0;
	while (o->t==SLL_OBJECT_TYPE_NOP){
		eoff++;
		o++;
	}
	switch (o->t){
		case SLL_OBJECT_TYPE_UNKNOWN:
		case SLL_OBJECT_TYPE_CHAR:
		case SLL_OBJECT_TYPE_IDENTIFIER:
		case SLL_OBJECT_TYPE_INT:
		case SLL_OBJECT_TYPE_FLOAT:
			return eoff+1;
		case SLL_OBJECT_TYPE_STRING:
			*(m+(o->dt.s>>6))|=1ull<<(o->dt.s&63);
			return eoff+1;
		case SLL_OBJECT_TYPE_FUNC:
		case SLL_OBJECT_TYPE_INTERNAL_FUNC:
			{
				sll_object_offset_t off=1;
				sll_arg_count_t l=o->dt.fn.ac;
				while (l){
					l--;
					off+=_mark_strings(o+off,m);
				}
				return off+eoff;
			}
		case SLL_OBJECT_TYPE_OPERATION_LIST:
			{
				sll_object_offset_t off=1;
				sll_statement_count_t l=o->dt.sc;
				while (l){
					l--;
					off+=_mark_strings(o+off,m);
				}
				return off+eoff;
			}
		case SLL_OBJECT_TYPE_DEBUG_DATA:
			*(m+(o->dt.dbg.fpi>>6))|=1ull<<(o->dt.dbg.fpi&63);
			return eoff+_mark_strings(o+1,m)+1;
	}
	sll_object_offset_t off=1;
	sll_arg_count_t l=o->dt.ac;
	while (l){
		l--;
		off+=_mark_strings(o+off,m);
	}
	return off+eoff;
}



sll_object_offset_t _update_strings(sll_object_t* o,sll_string_index_t* sm){
	sll_object_offset_t eoff=0;
	while (o->t==SLL_OBJECT_TYPE_NOP){
		eoff++;
		o++;
	}
	switch (o->t){
		case SLL_OBJECT_TYPE_UNKNOWN:
		case SLL_OBJECT_TYPE_CHAR:
		case SLL_OBJECT_TYPE_IDENTIFIER:
		case SLL_OBJECT_TYPE_INT:
		case SLL_OBJECT_TYPE_FLOAT:
			return eoff+1;
		case SLL_OBJECT_TYPE_STRING:
			o->dt.s=*(sm+o->dt.s);
			return eoff+1;
		case SLL_OBJECT_TYPE_FUNC:
		case SLL_OBJECT_TYPE_INTERNAL_FUNC:
			{
				sll_object_offset_t off=1;
				sll_arg_count_t l=o->dt.fn.ac;
				while (l){
					l--;
					off+=_update_strings(o+off,sm);
				}
				return off+eoff;
			}
		case SLL_OBJECT_TYPE_OPERATION_LIST:
			{
				sll_object_offset_t off=1;
				sll_statement_count_t l=o->dt.sc;
				while (l){
					l--;
					off+=_update_strings(o+off,sm);
				}
				return off+eoff;
			}
		case SLL_OBJECT_TYPE_DEBUG_DATA:
			o->dt.dbg.fpi=*(sm+o->dt.dbg.fpi);
			return eoff+_update_strings(o+1,sm)+1;
	}
	sll_object_offset_t off=1;
	sll_arg_count_t l=o->dt.ac;
	while (l){
		l--;
		off+=_update_strings(o+off,sm);
	}
	return off+eoff;
}



__SLL_FUNC void sll_optimize_metadata(sll_compilation_data_t* c_dt){
	uint32_t ml=(c_dt->st.l>>6)+1;
	uint64_t* m=malloc(ml*sizeof(uint64_t));
	for (uint32_t i=0;i<ml;i++){
		*(m+i)=0;
	}
	for (uint8_t i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		sll_identifier_list_t* l=c_dt->idt.s+i;
		for (sll_identifier_list_length_t j=0;j<l->l;j++){
			*(m+((l->dt+j)->i>>6))|=1ull<<((l->dt+j)->i&63);
		}
	}
	for (sll_identifier_list_length_t i=0;i<c_dt->idt.ill;i++){
		*(m+((c_dt->idt.il+i)->i>>6))|=1ull<<((c_dt->idt.il+i)->i&63);
	}
	_mark_strings(c_dt->h,m);
	sll_string_index_t* sm=malloc(c_dt->st.l*sizeof(sll_string_index_t));
	uint32_t k=0;
	uint32_t l=0;
	for (uint32_t i=0;i<ml;i++){
		uint64_t v=~(*(m+i));
		while (v){
			sll_string_index_t j=FIND_FIRST_SET_BIT(v)|(i<<6);
			if (j==c_dt->st.l){
				break;
			}
			SLL_RELEASE(*(c_dt->st.dt+j));
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
		c_dt->st.dt=realloc(c_dt->st.dt,c_dt->st.l*sizeof(sll_string_t*));
		for (uint8_t i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
			sll_identifier_list_t* e=c_dt->idt.s+i;
			for (sll_identifier_list_length_t j=0;j<e->l;j++){
				(e->dt+j)->i=*(sm+(e->dt+j)->i);
			}
		}
		for (sll_identifier_list_length_t i=0;i<c_dt->idt.ill;i++){
			(c_dt->idt.il+i)->i=*(sm+(c_dt->idt.il+i)->i);
		}
		_update_strings(c_dt->h,sm);
	}
	free(sm);
}

#include <sll/_internal/intrinsics.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/identifier.h>
#include <sll/memory.h>
#include <sll/node.h>
#include <sll/types.h>



static sll_node_t* _mark(sll_node_t* o,bitmap_t* m){
	while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DBG||o->t==SLL_NODE_TYPE_CHANGE_STACK){
		if (o->t==SLL_NODE_TYPE_CHANGE_STACK){
			o=o->dt._p;
		}
		else{
			if (o->t==SLL_NODE_TYPE_DBG&&o->dt.s!=SLL_MAX_STRING_INDEX){
				*(m+(o->dt.s>>6))|=1ull<<(o->dt.s&63);
			}
			o++;
		}
	}
	switch (o->t){
		case SLL_NODE_TYPE_CHAR:
		case SLL_NODE_TYPE_INT:
		case SLL_NODE_TYPE_FLOAT:
		case SLL_NODE_TYPE_IDENTIFIER:
		case SLL_NODE_TYPE_FUNCTION_ID:
			return o+1;
		case SLL_NODE_TYPE_STRING:
		case SLL_NODE_TYPE_FIELD:
			*(m+(o->dt.s>>6))|=1ull<<(o->dt.s&63);
			return o+1;
		case SLL_NODE_TYPE_FUNC:
		case SLL_NODE_TYPE_INTERNAL_FUNC:
			{
				sll_arg_count_t l=o->dt.fn.ac;
				o++;
				while (l){
					l--;
					o=_mark(o,m);
				}
				return o;
			}
		case SLL_NODE_TYPE_FOR:
		case SLL_NODE_TYPE_WHILE:
		case SLL_NODE_TYPE_LOOP:
		case SLL_NODE_TYPE_FOR_ARRAY:
		case SLL_NODE_TYPE_WHILE_ARRAY:
		case SLL_NODE_TYPE_FOR_MAP:
		case SLL_NODE_TYPE_WHILE_MAP:
			{
				sll_arg_count_t l=o->dt.l.ac;
				o++;
				while (l){
					l--;
					o=_mark(o,m);
				}
				return o;
			}
		case SLL_NODE_TYPE_DECL:
			{
				if (o->dt.d.nm!=SLL_MAX_STRING_INDEX){
					*(m+(o->dt.d.nm>>6))|=1ull<<(o->dt.d.nm&63);
				}
				sll_arg_count_t l=o->dt.d.ac;
				o++;
				while (l){
					l--;
					o=_mark(o,m);
				}
				return o;
			}
	}
	sll_arg_count_t l=o->dt.ac;
	o++;
	while (l){
		l--;
		o=_mark(o,m);
	}
	return o;
}



static sll_node_t* _update(sll_node_t* o,sll_string_index_t* sm){
	while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DBG||o->t==SLL_NODE_TYPE_CHANGE_STACK){
		if (o->t==SLL_NODE_TYPE_CHANGE_STACK){
			o=o->dt._p;
		}
		else{
			if (o->t==SLL_NODE_TYPE_DBG&&o->dt.s!=SLL_MAX_STRING_INDEX){
				o->dt.s=*(sm+o->dt.s);
			}
			o++;
		}
	}
	switch (o->t){
		case SLL_NODE_TYPE_CHAR:
		case SLL_NODE_TYPE_INT:
		case SLL_NODE_TYPE_FLOAT:
		case SLL_NODE_TYPE_IDENTIFIER:
		case SLL_NODE_TYPE_FUNCTION_ID:
			return o+1;
		case SLL_NODE_TYPE_STRING:
		case SLL_NODE_TYPE_FIELD:
			o->dt.s=*(sm+o->dt.s);
			return o+1;
		case SLL_NODE_TYPE_FUNC:
		case SLL_NODE_TYPE_INTERNAL_FUNC:
			{
				sll_arg_count_t l=o->dt.fn.ac;
				o++;
				while (l){
					l--;
					o=_update(o,sm);
				}
				return o;
			}
		case SLL_NODE_TYPE_FOR:
		case SLL_NODE_TYPE_WHILE:
		case SLL_NODE_TYPE_LOOP:
		case SLL_NODE_TYPE_FOR_ARRAY:
		case SLL_NODE_TYPE_WHILE_ARRAY:
		case SLL_NODE_TYPE_FOR_MAP:
		case SLL_NODE_TYPE_WHILE_MAP:
			{
				sll_arg_count_t l=o->dt.l.ac;
				o++;
				while (l){
					l--;
					o=_update(o,sm);
				}
				return o;
			}
		case SLL_NODE_TYPE_DECL:
			{
				if (o->dt.d.nm!=SLL_MAX_STRING_INDEX){
					o->dt.d.nm=*(sm+o->dt.d.nm);
				}
				sll_arg_count_t l=o->dt.d.ac;
				o++;
				while (l){
					l--;
					o=_update(o,sm);
				}
				return o;
			}
	}
	sll_arg_count_t l=o->dt.ac;
	o++;
	while (l){
		l--;
		o=_update(o,sm);
	}
	return o;
}



__SLL_EXTERNAL void sll_optimize_metadata(sll_compilation_data_t* c_dt){
	for (sll_source_file_index_t idx=0;idx<c_dt->l;idx++){
		sll_source_file_t* sf=*(c_dt->dt+idx);
		if (!sf->st.l){
			continue;
		}
		sll_string_index_t ml=(sf->st.l>>6)+1;
		bitmap_t* m=sll_zero_allocate_stack(ml*sizeof(bitmap_t));
		*(m+(sf->fp_nm>>6))|=1ull<<(sf->fp_nm&63);
		for (sll_identifier_index_t i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
			sll_identifier_list_t* l=sf->idt.s+i;
			for (sll_identifier_list_length_t j=0;j<l->l;j++){
				*(m+(SLL_IDENTIFIER_GET_STRING_INDEX(l->dt+j)>>6))|=1ull<<(SLL_IDENTIFIER_GET_STRING_INDEX(l->dt+j)&63);
			}
		}
		for (sll_identifier_list_length_t i=0;i<sf->idt.ill;i++){
			*(m+(SLL_IDENTIFIER_GET_STRING_INDEX(sf->idt.il+i)>>6))|=1ull<<(SLL_IDENTIFIER_GET_STRING_INDEX(sf->idt.il+i)&63);
		}
		for (sll_function_index_t i=0;i<sf->ft.l;i++){
			sll_string_index_t j=(*(sf->ft.dt+i))->nm;
			if (j!=SLL_MAX_STRING_INDEX){
				*(m+(j>>6))|=1ull<<(j&63);
			}
		}
		_mark(sf->dt,m);
		sll_string_index_t* sm=sll_allocate_stack(sf->st.l*sizeof(sll_string_index_t));
		sll_string_index_t k=0;
		sll_string_index_t l=0;
		for (sll_string_index_t i=0;i<ml;i++){
			bitmap_t v=~(*(m+i));
			while (v){
				sll_string_index_t j=FIND_FIRST_SET_BIT(v)|(i<<6);
				if (j==sf->st.l){
					break;
				}
				sll_free_string(sf->st.dt+j);
				for (sll_string_index_t n=k;n<j;n++){
					*(sf->st.dt+n-l)=*(sf->st.dt+n);
					*(sm+n)=n-l;
				}
				k=j+1;
				l++;
				v&=v-1;
			}
		}
		sll_deallocate(m);
		for (sll_string_index_t i=k;i<sf->st.l;i++){
			*(sf->st.dt+i-l)=*(sf->st.dt+i);
			*(sm+i)=i-l;
		}
		if (l){
			sf->st.l-=l;
			sf->st.dt=sll_reallocate(sf->st.dt,sf->st.l*sizeof(sll_string_t));
			for (sll_identifier_index_t i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
				sll_identifier_list_t* e=sf->idt.s+i;
				for (sll_identifier_list_length_t j=0;j<e->l;j++){
					SLL_IDENTIFIER_UPDATE_STRING_INDEX(e->dt+j,*(sm+SLL_IDENTIFIER_GET_STRING_INDEX(e->dt+j)));
				}
			}
			for (sll_identifier_list_length_t i=0;i<sf->idt.ill;i++){
				SLL_IDENTIFIER_UPDATE_STRING_INDEX(sf->idt.il+i,*(sm+SLL_IDENTIFIER_GET_STRING_INDEX(sf->idt.il+i)));
			}
			for (sll_function_index_t i=0;i<sf->ft.l;i++){
				if ((*(sf->ft.dt+i))->nm!=SLL_MAX_STRING_INDEX){
					(*(sf->ft.dt+i))->nm=*(sm+(*(sf->ft.dt+i))->nm);
				}
			}
			sf->fp_nm=*(sm+sf->fp_nm);
			_update(sf->dt,sm);
		}
		sll_deallocate(sm);
	}
}

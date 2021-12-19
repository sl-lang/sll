#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/memory.h>
#include <sll/node.h>
#include <sll/types.h>
#include <stdint.h>



static sll_node_t* _mark(sll_node_t* o,uint64_t* m){
	while (o->t==SLL_NODE_TYPE_NOP||o->t==NODE_TYPE_CHANGE_STACK){
		o=(o->t==NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
	}
	switch (o->t){
		case SLL_NODE_TYPE_UNKNOWN:
		case SLL_NODE_TYPE_CHAR:
		case SLL_NODE_TYPE_IDENTIFIER:
		case SLL_NODE_TYPE_INT:
		case SLL_NODE_TYPE_FLOAT:
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
		case SLL_NODE_TYPE_DECL_COPY:
			if (o->dt.dc.nm!=SLL_MAX_STRING_INDEX){
				*(m+(o->dt.dc.nm>>6))|=1ull<<(o->dt.dc.nm&63);
			}
			return o+1;
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
	while (o->t==SLL_NODE_TYPE_NOP||o->t==NODE_TYPE_CHANGE_STACK){
		o=(o->t==NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
	}
	switch (o->t){
		case SLL_NODE_TYPE_UNKNOWN:
		case SLL_NODE_TYPE_CHAR:
		case SLL_NODE_TYPE_IDENTIFIER:
		case SLL_NODE_TYPE_INT:
		case SLL_NODE_TYPE_FLOAT:
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
		case SLL_NODE_TYPE_DECL_COPY:
			if (o->dt.dc.nm!=SLL_MAX_STRING_INDEX){
				o->dt.dc.nm=*(sm+o->dt.dc.nm);
			}
			return o+1;
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
	uint32_t ml=(c_dt->st.l>>6)+1;
	uint64_t* m=sll_zero_allocate_stack(ml*sizeof(uint64_t));
	for (uint8_t i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		sll_identifier_list_t* l=c_dt->idt.s+i;
		for (sll_identifier_list_length_t j=0;j<l->l;j++){
			*(m+((l->dt+j)->i>>6))|=1ull<<((l->dt+j)->i&63);
		}
	}
	for (sll_identifier_list_length_t i=0;i<c_dt->idt.ill;i++){
		*(m+((c_dt->idt.il+i)->i>>6))|=1ull<<((c_dt->idt.il+i)->i&63);
	}
	for (sll_function_index_t i=0;i<c_dt->ft.l;i++){
		sll_string_index_t j=(*(c_dt->ft.dt+i))->nm;
		if (j!=SLL_MAX_STRING_INDEX){
			*(m+(j>>6))|=1ull<<(j&63);
		}
	}
	for (sll_object_type_t i=0;i<c_dt->ot_it.l;i++){
		sll_object_type_initializer_t* oi=*(c_dt->ot_it.dt+i);
		SLL_ASSERT(oi->l);
		for (sll_arg_count_t j=0;j<oi->l;j++){
			SLL_ASSERT(oi->dt[j].f<c_dt->st.l);
			*(m+(oi->dt[j].f>>6))|=1ull<<(oi->dt[j].f&63);
		}
	}
	_mark(c_dt->h,m);
	sll_string_index_t* sm=sll_allocate_stack(c_dt->st.l*sizeof(sll_string_index_t));
	uint32_t k=0;
	uint32_t l=0;
	for (uint32_t i=0;i<ml;i++){
		uint64_t v=~(*(m+i));
		while (v){
			sll_string_index_t j=FIND_FIRST_SET_BIT(v)|(i<<6);
			if (j==c_dt->st.l){
				break;
			}
			sll_free_string(c_dt->st.dt+j);
			for (uint32_t n=k;n<j;n++){
				*(c_dt->st.dt+n-l)=*(c_dt->st.dt+n);
				*(sm+n)=n-l;
			}
			k=j+1;
			l++;
			v&=v-1;
		}
	}
	sll_deallocate(m);
	for (uint32_t i=k;i<c_dt->st.l;i++){
		*(c_dt->st.dt+i-l)=*(c_dt->st.dt+i);
		*(sm+i)=i-l;
	}
	if (l){
		c_dt->st.l-=l;
		c_dt->st.dt=sll_reallocate(c_dt->st.dt,c_dt->st.l*sizeof(sll_string_t));
		for (uint8_t i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
			sll_identifier_list_t* e=c_dt->idt.s+i;
			for (sll_identifier_list_length_t j=0;j<e->l;j++){
				(e->dt+j)->i=*(sm+(e->dt+j)->i);
			}
		}
		for (sll_identifier_list_length_t i=0;i<c_dt->idt.ill;i++){
			(c_dt->idt.il+i)->i=*(sm+(c_dt->idt.il+i)->i);
		}
		for (sll_function_index_t i=0;i<c_dt->ft.l;i++){
			if ((*(c_dt->ft.dt+i))->nm!=SLL_MAX_STRING_INDEX){
				(*(c_dt->ft.dt+i))->nm=*(sm+(*(c_dt->ft.dt+i))->nm);
			}
		}
		for (sll_object_type_t i=0;i<c_dt->ot_it.l;i++){
			sll_object_type_initializer_t* oi=*(c_dt->ot_it.dt+i);
			SLL_ASSERT(oi->l);
			for (sll_arg_count_t j=0;j<oi->l;j++){
				oi->dt[j].f=*(sm+oi->dt[j].f);
			}
		}
		_update(c_dt->h,sm);
	}
	sll_deallocate(sm);
}

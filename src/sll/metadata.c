#include <sll/_internal/common.h>
#include <sll/_internal/intrinsics.h>
#include <sll/common.h>
#include <sll/identifier.h>
#include <sll/memory.h>
#include <sll/node.h>
#include <sll/string.h>
#include <sll/string_table.h>
#include <sll/types.h>



static sll_node_t* _mark(sll_node_t* o,bitmap_t* m){
	while (o->type==SLL_NODE_TYPE_NOP||o->type==SLL_NODE_TYPE_DBG||o->type==SLL_NODE_TYPE_CHANGE_STACK){
		if (o->type==SLL_NODE_TYPE_CHANGE_STACK){
			o=o->data._next_node;
		}
		else{
			if (o->type==SLL_NODE_TYPE_DBG&&o->data.string_index!=SLL_MAX_STRING_INDEX){
				*(m+(o->data.string_index>>6))|=1ull<<(o->data.string_index&63);
			}
			o++;
		}
	}
	switch (o->type){
		case SLL_NODE_TYPE_INT:
		case SLL_NODE_TYPE_FLOAT:
		case SLL_NODE_TYPE_CHAR:
		case SLL_NODE_TYPE_COMPLEX:
		case SLL_NODE_TYPE_IDENTIFIER:
		case SLL_NODE_TYPE_FUNCTION_ID:
			return o+1;
		case SLL_NODE_TYPE_STRING:
		case SLL_NODE_TYPE_FIELD:
			*(m+(o->data.string_index>>6))|=1ull<<(o->data.string_index&63);
			return o+1;
		case SLL_NODE_TYPE_FUNC:
		case SLL_NODE_TYPE_INTERNAL_FUNC:
			{
				sll_arg_count_t l=o->data.function.arg_count;
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
				sll_arg_count_t l=o->data.loop.arg_count;
				o++;
				while (l){
					l--;
					o=_mark(o,m);
				}
				return o;
			}
		case SLL_NODE_TYPE_DECL:
			{
				if (o->data.declaration.name_string_index!=SLL_MAX_STRING_INDEX){
					*(m+(o->data.declaration.name_string_index>>6))|=1ull<<(o->data.declaration.name_string_index&63);
				}
				sll_arg_count_t l=o->data.declaration.arg_count;
				o++;
				while (l){
					l--;
					o=_mark(o,m);
				}
				return o;
			}
	}
	sll_arg_count_t l=o->data.arg_count;
	o++;
	while (l){
		l--;
		o=_mark(o,m);
	}
	return o;
}



static sll_node_t* _update(sll_node_t* o,sll_string_index_t* sm){
	while (o->type==SLL_NODE_TYPE_NOP||o->type==SLL_NODE_TYPE_DBG||o->type==SLL_NODE_TYPE_CHANGE_STACK){
		if (o->type==SLL_NODE_TYPE_CHANGE_STACK){
			o=o->data._next_node;
		}
		else{
			if (o->type==SLL_NODE_TYPE_DBG&&o->data.string_index!=SLL_MAX_STRING_INDEX){
				o->data.string_index=*(sm+o->data.string_index);
			}
			o++;
		}
	}
	switch (o->type){
		case SLL_NODE_TYPE_INT:
		case SLL_NODE_TYPE_FLOAT:
		case SLL_NODE_TYPE_CHAR:
		case SLL_NODE_TYPE_COMPLEX:
		case SLL_NODE_TYPE_IDENTIFIER:
		case SLL_NODE_TYPE_FUNCTION_ID:
			return o+1;
		case SLL_NODE_TYPE_STRING:
		case SLL_NODE_TYPE_FIELD:
			o->data.string_index=*(sm+o->data.string_index);
			return o+1;
		case SLL_NODE_TYPE_FUNC:
		case SLL_NODE_TYPE_INTERNAL_FUNC:
			{
				sll_arg_count_t l=o->data.function.arg_count;
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
				sll_arg_count_t l=o->data.loop.arg_count;
				o++;
				while (l){
					l--;
					o=_update(o,sm);
				}
				return o;
			}
		case SLL_NODE_TYPE_DECL:
			{
				if (o->data.declaration.name_string_index!=SLL_MAX_STRING_INDEX){
					o->data.declaration.name_string_index=*(sm+o->data.declaration.name_string_index);
				}
				sll_arg_count_t l=o->data.declaration.arg_count;
				o++;
				while (l){
					l--;
					o=_update(o,sm);
				}
				return o;
			}
	}
	sll_arg_count_t l=o->data.arg_count;
	o++;
	while (l){
		l--;
		o=_update(o,sm);
	}
	return o;
}



__SLL_EXTERNAL void sll_optimize_metadata(sll_compilation_data_t* c_dt){
	for (sll_source_file_index_t idx=0;idx<c_dt->length;idx++){
		sll_source_file_t* sf=*(c_dt->data+idx);
		if (!sf->string_table.l){
			continue;
		}
		sll_string_index_t ml=(sf->string_table.l>>6)+1;
		bitmap_t* m=sll_zero_allocate_stack(ml*sizeof(bitmap_t));
		*(m+(sf->file_path_string_index>>6))|=1ull<<(sf->file_path_string_index&63);
		for (sll_identifier_index_t i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
			sll_identifier_list_t* l=sf->identifier_table.short_+i;
			for (sll_identifier_list_length_t j=0;j<l->length;j++){
				*(m+(SLL_IDENTIFIER_GET_STRING_INDEX(l->data+j)>>6))|=1ull<<(SLL_IDENTIFIER_GET_STRING_INDEX(l->data+j)&63);
			}
		}
		for (sll_identifier_list_length_t i=0;i<sf->identifier_table.long_data_length;i++){
			*(m+(SLL_IDENTIFIER_GET_STRING_INDEX(sf->identifier_table.long_data+i)>>6))|=1ull<<(SLL_IDENTIFIER_GET_STRING_INDEX(sf->identifier_table.long_data+i)&63);
		}
		for (sll_function_index_t i=0;i<sf->function_table.length;i++){
			sll_string_index_t j=(*(sf->function_table.data+i))->name_string_index;
			if (j!=SLL_MAX_STRING_INDEX){
				*(m+(j>>6))|=1ull<<(j&63);
			}
			j=(*(sf->function_table.data+i))->description_string_index;
			if (j!=SLL_MAX_STRING_INDEX){
				*(m+(j>>6))|=1ull<<(j&63);
			}
		}
		if (sf->first_node){
			_mark(sf->first_node,m);
		}
		sll_string_index_t* sm=sll_allocate_stack(sf->string_table.l*sizeof(sll_string_index_t));
		sll_string_index_t k=0;
		sll_string_index_t l=0;
		for (sll_string_index_t i=0;i<ml;i++){
			bitmap_t v=~(*(m+i));
			while (v){
				sll_string_index_t j=FIND_FIRST_SET_BIT(v)|(i<<6);
				if (j==sf->string_table.l){
					break;
				}
				sll_free_string(sf->string_table.dt+j);
				for (sll_string_index_t n=k;n<j;n++){
					*(sf->string_table.dt+n-l)=*(sf->string_table.dt+n);
					*(sm+n)=n-l;
				}
				k=j+1;
				l++;
				v&=v-1;
			}
		}
		sll_deallocate(m);
		for (sll_string_index_t i=k;i<sf->string_table.l;i++){
			*(sf->string_table.dt+i-l)=*(sf->string_table.dt+i);
			*(sm+i)=i-l;
		}
		if (l){
			sf->string_table.l-=l;
			sf->string_table.dt=sll_reallocate(sf->string_table.dt,sf->string_table.l*sizeof(sll_string_t));
			for (sll_identifier_index_t i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
				sll_identifier_list_t* e=sf->identifier_table.short_+i;
				for (sll_identifier_list_length_t j=0;j<e->length;j++){
					SLL_IDENTIFIER_UPDATE_STRING_INDEX(e->data+j,*(sm+SLL_IDENTIFIER_GET_STRING_INDEX(e->data+j)));
				}
			}
			for (sll_identifier_list_length_t i=0;i<sf->identifier_table.long_data_length;i++){
				SLL_IDENTIFIER_UPDATE_STRING_INDEX(sf->identifier_table.long_data+i,*(sm+SLL_IDENTIFIER_GET_STRING_INDEX(sf->identifier_table.long_data+i)));
			}
			for (sll_function_index_t i=0;i<sf->function_table.length;i++){
				if ((*(sf->function_table.data+i))->name_string_index!=SLL_MAX_STRING_INDEX){
					(*(sf->function_table.data+i))->name_string_index=*(sm+(*(sf->function_table.data+i))->name_string_index);
				}
				if ((*(sf->function_table.data+i))->description_string_index!=SLL_MAX_STRING_INDEX){
					(*(sf->function_table.data+i))->description_string_index=*(sm+(*(sf->function_table.data+i))->description_string_index);
				}
			}
			sf->file_path_string_index=*(sm+sf->file_path_string_index);
			if (sf->first_node){
				_update(sf->first_node,sm);
			}
		}
		sll_deallocate(sm);
	}
}

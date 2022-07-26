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
		case SLL_NODE_TYPE_STRING:
		case SLL_NODE_TYPE_FIELD:
			*(m+(o->data.string_index>>6))|=1ull<<(o->data.string_index&63);
		case SLL_NODE_TYPE_INT:
		case SLL_NODE_TYPE_FLOAT:
		case SLL_NODE_TYPE_CHAR:
		case SLL_NODE_TYPE_COMPLEX:
		case SLL_NODE_TYPE_IDENTIFIER:
		case SLL_NODE_TYPE_FUNCTION_ID:
			return o+1;
		case SLL_NODE_TYPE_DECL:
			if (o->data.declaration.name_string_index!=SLL_MAX_STRING_INDEX){
				*(m+(o->data.declaration.name_string_index>>6))|=1ull<<(o->data.declaration.name_string_index&63);
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
		case SLL_NODE_TYPE_STRING:
		case SLL_NODE_TYPE_FIELD:
			o->data.string_index=*(sm+o->data.string_index);
		case SLL_NODE_TYPE_INT:
		case SLL_NODE_TYPE_FLOAT:
		case SLL_NODE_TYPE_CHAR:
		case SLL_NODE_TYPE_COMPLEX:
		case SLL_NODE_TYPE_IDENTIFIER:
		case SLL_NODE_TYPE_FUNCTION_ID:
			return o+1;
		case SLL_NODE_TYPE_DECL:
			if (o->data.declaration.name_string_index!=SLL_MAX_STRING_INDEX){
				o->data.declaration.name_string_index=*(sm+o->data.declaration.name_string_index);
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



__SLL_EXTERNAL void sll_optimize_metadata(sll_compilation_data_t* compilation_data){
	for (sll_source_file_index_t idx=0;idx<compilation_data->length;idx++){
		sll_source_file_t* sf=*(compilation_data->data+idx);
		if (!sf->string_table.length){
			continue;
		}
		sll_string_index_t ml=(sf->string_table.length>>6)+1;
		bitmap_t* m=sll_zero_allocate_stack(ml*sizeof(bitmap_t));
		*(m+(sf->file_path_string_index>>6))|=1ull<<(sf->file_path_string_index&63);
		for (sll_identifier_table_length_t i=0;i<sf->identifier_table.length;i++){
			*(m+(SLL_IDENTIFIER_GET_STRING_INDEX(sf->identifier_table.data+i)>>6))|=1ull<<(SLL_IDENTIFIER_GET_STRING_INDEX(sf->identifier_table.data+i)&63);
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
		sll_string_index_t* sm=sll_allocate_stack(sf->string_table.length*sizeof(sll_string_index_t));
		sll_string_index_t k=0;
		sll_string_index_t l=0;
		for (sll_string_index_t i=0;i<ml;i++){
			bitmap_t v=~(*(m+i));
			while (v){
				sll_string_index_t j=FIND_FIRST_SET_BIT(v)|(i<<6);
				if (j==sf->string_table.length){
					break;
				}
				sll_free_string(sf->string_table.data+j);
				for (sll_string_index_t n=k;n<j;n++){
					*(sf->string_table.data+n-l)=*(sf->string_table.data+n);
					*(sm+n)=n-l;
				}
				k=j+1;
				l++;
				v&=v-1;
			}
		}
		sll_deallocate(m);
		for (sll_string_index_t i=k;i<sf->string_table.length;i++){
			*(sf->string_table.data+i-l)=*(sf->string_table.data+i);
			*(sm+i)=i-l;
		}
		if (l){
			sf->string_table.length-=l;
			sf->string_table.data=sll_reallocate(sf->string_table.data,sf->string_table.length*sizeof(sll_string_t));
			for (sll_identifier_table_length_t i=0;i<sf->identifier_table.length;i++){
				SLL_IDENTIFIER_UPDATE_STRING_INDEX(sf->identifier_table.data+i,*(sm+SLL_IDENTIFIER_GET_STRING_INDEX(sf->identifier_table.data+i)));
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

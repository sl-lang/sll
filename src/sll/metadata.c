#include <sll/_internal/common.h>
#include <sll/_internal/intrinsics.h>
#include <sll/common.h>
#include <sll/identifier.h>
#include <sll/memory.h>
#include <sll/node.h>
#include <sll/string.h>
#include <sll/string_table.h>
#include <sll/types.h>



static sll_node_t _mark(sll_node_t node,bitmap_t* bitmap){
	while (node->type==SLL_NODE_TYPE_NOP||node->type==SLL_NODE_TYPE_DBG||node->type==SLL_NODE_TYPE_CHANGE_STACK){
		if (node->type==SLL_NODE_TYPE_CHANGE_STACK){
			node=node->data._next_node;
		}
		else{
			if (node->type==SLL_NODE_TYPE_DBG&&node->data.string_index!=SLL_MAX_STRING_INDEX){
				*(bitmap+(node->data.string_index>>6))|=1ull<<(node->data.string_index&63);
			}
			node++;
		}
	}
	switch (node->type){
		case SLL_NODE_TYPE_STRING:
		case SLL_NODE_TYPE_FIELD:
			*(bitmap+(node->data.string_index>>6))|=1ull<<(node->data.string_index&63);
		case SLL_NODE_TYPE_INT:
		case SLL_NODE_TYPE_FLOAT:
		case SLL_NODE_TYPE_CHAR:
		case SLL_NODE_TYPE_COMPLEX:
		case SLL_NODE_TYPE_IDENTIFIER:
		case SLL_NODE_TYPE_FUNCTION_ID:
			return node+1;
		case SLL_NODE_TYPE_DECL:
			if (node->data.declaration.name_string_index!=SLL_MAX_STRING_INDEX){
				*(bitmap+(node->data.declaration.name_string_index>>6))|=1ull<<(node->data.declaration.name_string_index&63);
			}
	}
	sll_arg_count_t arg_count=node->data.arg_count;
	node++;
	while (arg_count){
		arg_count--;
		node=_mark(node,bitmap);
	}
	return node;
}



__SLL_EXTERNAL void sll_optimize_metadata(sll_compilation_data_t* compilation_data){
	for (sll_source_file_index_t idx=0;idx<compilation_data->length;idx++){
		sll_source_file_t* source_file=*(compilation_data->data+idx);
		if (!source_file->string_table.length){
			continue;
		}
		sll_string_index_t ml=(source_file->string_table.length>>6)+1;
		bitmap_t* m=sll_zero_allocate_stack(ml*sizeof(bitmap_t));
		*(m+(source_file->file_path_string_index>>6))|=1ull<<(source_file->file_path_string_index&63);
		for (sll_identifier_table_length_t i=0;i<source_file->identifier_table.length;i++){
			*(m+(SLL_IDENTIFIER_GET_STRING_INDEX(source_file->identifier_table.data+i)>>6))|=1ull<<(SLL_IDENTIFIER_GET_STRING_INDEX(source_file->identifier_table.data+i)&63);
		}
		for (sll_function_index_t i=0;i<source_file->function_table.length;i++){
			sll_string_index_t j=(*(source_file->function_table.data+i))->name_string_index;
			if (j!=SLL_MAX_STRING_INDEX){
				*(m+(j>>6))|=1ull<<(j&63);
			}
			j=(*(source_file->function_table.data+i))->description_string_index;
			if (j!=SLL_MAX_STRING_INDEX){
				*(m+(j>>6))|=1ull<<(j&63);
			}
		}
		if (source_file->first_node){
			_mark(source_file->first_node,m);
		}
		sll_string_index_t* string_map=sll_allocate_stack(source_file->string_table.length*sizeof(sll_string_index_t));
		sll_string_index_t k=0;
		sll_string_index_t l=0;
		for (sll_string_index_t i=0;i<ml;i++){
			bitmap_t v=~(*(m+i));
			while (v){
				sll_string_index_t j=FIND_FIRST_SET_BIT(v)|(i<<6);
				if (j==source_file->string_table.length){
					break;
				}
				sll_free_string(source_file->string_table.data+j);
				for (sll_string_index_t n=k;n<j;n++){
					*(source_file->string_table.data+n-l)=*(source_file->string_table.data+n);
					*(string_map+n)=n-l;
				}
				k=j+1;
				l++;
				v&=v-1;
			}
		}
		sll_deallocate(m);
		for (sll_string_index_t i=k;i<source_file->string_table.length;i++){
			*(source_file->string_table.data+i-l)=*(source_file->string_table.data+i);
			*(string_map+i)=i-l;
		}
		if (l){
			source_file->string_table.length-=l;
			source_file->string_table.data=sll_reallocate(source_file->string_table.data,source_file->string_table.length*sizeof(sll_string_t));
			for (sll_identifier_table_length_t i=0;i<source_file->identifier_table.length;i++){
				SLL_IDENTIFIER_UPDATE_STRING_INDEX(source_file->identifier_table.data+i,*(string_map+SLL_IDENTIFIER_GET_STRING_INDEX(source_file->identifier_table.data+i)));
			}
			for (sll_function_index_t i=0;i<source_file->function_table.length;i++){
				if ((*(source_file->function_table.data+i))->name_string_index!=SLL_MAX_STRING_INDEX){
					(*(source_file->function_table.data+i))->name_string_index=*(string_map+(*(source_file->function_table.data+i))->name_string_index);
				}
				if ((*(source_file->function_table.data+i))->description_string_index!=SLL_MAX_STRING_INDEX){
					(*(source_file->function_table.data+i))->description_string_index=*(string_map+(*(source_file->function_table.data+i))->description_string_index);
				}
			}
			source_file->file_path_string_index=*(string_map+source_file->file_path_string_index);
			if (source_file->first_node){
				sll_node_t node=source_file->first_node;
				sll_node_offset_t stack=1;
				while (stack){
					if (node->type==SLL_NODE_TYPE_CHANGE_STACK){
						node=node->data._next_node;
					}
					if (node->type==SLL_NODE_TYPE_DBG){
						if (node->data.string_index!=SLL_MAX_STRING_INDEX){
							node->data.string_index=*(string_map+node->data.string_index);
						}
					}
					else{
						stack--;
					}
					if (SLL_NODE_HAS_CHILDREN(node)&&node->type<SLL_NODE_TYPE_DBG){
						stack+=node->data.arg_count;
						if (node->type==SLL_NODE_TYPE_DECL&&node->data.declaration.name_string_index!=SLL_MAX_STRING_INDEX){
							node->data.declaration.name_string_index=*(string_map+node->data.declaration.name_string_index);
						}
					}
					else if (node->type==SLL_NODE_TYPE_STRING||node->type==SLL_NODE_TYPE_FIELD){
						node->data.string_index=*(string_map+node->data.string_index);
					}
					node++;
				}
			}
		}
		sll_deallocate(string_map);
	}
}

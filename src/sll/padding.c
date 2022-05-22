#include <sll/common.h>
#include <sll/node.h>
#include <sll/types.h>



static sll_node_t* _remove_padding_internal(sll_node_t* src,sll_source_file_t* source_file,sll_node_t** dst,sll_node_offset_t* removal_offset){
	while (src->type==SLL_NODE_TYPE_NOP||src->type==SLL_NODE_TYPE_CHANGE_STACK){
		if (src->type==SLL_NODE_TYPE_CHANGE_STACK){
			src=src->data._next_node;
		}
		else{
			src++;
			(*removal_offset)++;
		}
	}
	**dst=*src;
	(*dst)++;
	if ((*dst)->type==SLL_NODE_TYPE_CHANGE_STACK){
		(*dst)=(*dst)->data._next_node;
	}
	switch (src->type){
		case SLL_NODE_TYPE_INT:
		case SLL_NODE_TYPE_FLOAT:
		case SLL_NODE_TYPE_CHAR:
		case SLL_NODE_TYPE_COMPLEX:
		case SLL_NODE_TYPE_STRING:
		case SLL_NODE_TYPE_IDENTIFIER:
		case SLL_NODE_TYPE_FIELD:
		case SLL_NODE_TYPE_FUNCTION_ID:
			return src+1;
		case SLL_NODE_TYPE_ARRAY:
			{
				sll_array_length_t l=src->data.array_length;
				src++;
				while (l){
					l--;
					src=_remove_padding_internal(src,source_file,dst,removal_offset);
				}
				return src;
			}
		case SLL_NODE_TYPE_MAP:
			{
				sll_map_length_t l=src->data.map_length;
				src++;
				while (l){
					l--;
					src=_remove_padding_internal(src,source_file,dst,removal_offset);
				}
				return src;
			}
		case SLL_NODE_TYPE_FUNC:
			(*(source_file->function_table.data+src->data.function.function_index))->offset-=*removal_offset;
		case SLL_NODE_TYPE_INTERNAL_FUNC:
			{
				sll_arg_count_t l=src->data.function.arg_count;
				src++;
				while (l){
					l--;
					src=_remove_padding_internal(src,source_file,dst,removal_offset);
				}
				return src;
			}
		case SLL_NODE_TYPE_FOR:
		case SLL_NODE_TYPE_WHILE:
		case SLL_NODE_TYPE_LOOP:
		case SLL_NODE_TYPE_FOR_ARRAY:
		case SLL_NODE_TYPE_WHILE_ARRAY:
		case SLL_NODE_TYPE_FOR_MAP:
		case SLL_NODE_TYPE_WHILE_MAP:
			{
				sll_arg_count_t l=src->data.loop.arg_count;
				src++;
				while (l){
					l--;
					src=_remove_padding_internal(src,source_file,dst,removal_offset);
				}
				return src;
			}
		case SLL_NODE_TYPE_DECL:
			{
				sll_arg_count_t l=src->data.declaration.arg_count;
				src++;
				while (l){
					l--;
					src=_remove_padding_internal(src,source_file,dst,removal_offset);
				}
				return src;
			}
		case SLL_NODE_TYPE_DBG:
			return _remove_padding_internal(src+1,source_file,dst,removal_offset);
	}
	sll_arg_count_t l=src->data.arg_count;
	src++;
	while (l){
		l--;
		src=_remove_padding_internal(src,source_file,dst,removal_offset);
	}
	return src;
}



__SLL_EXTERNAL void sll_remove_node_padding(sll_compilation_data_t* compilation_data){
	for (sll_source_file_index_t i=0;i<compilation_data->length;i++){
		sll_source_file_t* sf=*(compilation_data->data+i);
		sll_node_t* d=sf->first_node;
		if (!d){
			continue;
		}
		sll_node_offset_t rm=0;
		_remove_padding_internal(d,sf,&d,&rm);
	}
}

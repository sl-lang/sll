#include <sll/common.h>
#include <sll/node.h>
#include <sll/types.h>



static sll_node_t _remove_padding_internal(sll_node_t src,sll_source_file_t* source_file,sll_node_t* dst,sll_node_offset_t* removal_offset){
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
	if (!SLL_NODE_HAS_CHILDREN(src)){
		return src+1;
	}
	sll_arg_count_t arg_count=src->data.arg_count;
	if (src->type==SLL_NODE_TYPE_FUNC){
		(*(source_file->function_table.data+src->data.function.function_index))->offset-=*removal_offset;
	}
	else if (src->type==SLL_NODE_TYPE_DBG){
		arg_count=1;
	}
	src++;
	while (arg_count){
		arg_count--;
		src=_remove_padding_internal(src,source_file,dst,removal_offset);
	}
	return src;
}



__SLL_EXTERNAL void sll_remove_node_padding(sll_compilation_data_t* compilation_data){
	for (sll_source_file_index_t i=0;i<compilation_data->length;i++){
		sll_source_file_t* sf=*(compilation_data->data+i);
		sll_node_t d=sf->first_node;
		if (!d){
			continue;
		}
		sll_node_offset_t rm=0;
		_remove_padding_internal(d,sf,&d,&rm);
	}
}

#include <sll/_internal/optimizer.h>
#include <sll/node.h>
#include <sll/string.h>
#include <sll/string_table.h>
#include <sll/types.h>



OPTIMIZER_FUNTION(optimize_print){
	if (node->type!=SLL_NODE_TYPE_PRINT){
		return node;
	}
	sll_arg_count_t ac=node->data.arg_count;
	sll_node_t* prev=NULL;
	for (sll_arg_count_t i=0;i<ac;i++){
		sll_node_t* child=*(children+i);
		if (child->type==SLL_NODE_TYPE_DBG||child->type==SLL_NODE_TYPE_NOP){
			continue;
		}
		if (child->type==SLL_NODE_TYPE_CHAR){
			if (prev){
				sll_string_t tmp;
				if (prev->type==SLL_NODE_TYPE_CHAR){
					sll_string_concat_chars(prev->data.char_,child->data.char_,&tmp);
				}
				else{
					sll_string_concat_char(source_file->string_table.data+prev->data.string_index,child->data.char_,&tmp);
				}
				prev->type=SLL_NODE_TYPE_NOP;
				node->data.arg_count--;
				child->type=SLL_NODE_TYPE_STRING;
				child->data.string_index=sll_add_string(&(source_file->string_table),&tmp);
			}
			prev=child;
		}
		else if (child->type==SLL_NODE_TYPE_STRING){
			sll_string_t* str=source_file->string_table.data+child->data.string_index;
			if (!str->length){
				child->type=SLL_NODE_TYPE_NOP;
				node->data.arg_count--;
			}
			else{
				if (prev){
					sll_string_t tmp;
					if (prev->type==SLL_NODE_TYPE_CHAR){
						sll_string_prepend_char(str,prev->data.char_,&tmp);
					}
					else{
						sll_string_concat(source_file->string_table.data+prev->data.string_index,str,&tmp);
					}
					prev->type=SLL_NODE_TYPE_NOP;
					node->data.arg_count--;
					child->data.string_index=sll_add_string(&(source_file->string_table),&tmp);
				}
				else if (str->length==1){
					child->type=SLL_NODE_TYPE_CHAR;
					child->data.char_=str->data[0];
				}
				prev=child;
			}
		}
		else{
			prev=NULL;
		}
	}
	return node;
}

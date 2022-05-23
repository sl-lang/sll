#include <sll/_internal/common.h>
#include <sll/_internal/optimizer.h>
#include <sll/node.h>
#include <sll/string.h>
#include <sll/string_table.h>
#include <sll/types.h>



sll_bool_t _merge_print_strings(sll_source_file_t* source_file,sll_node_t* node,sll_arg_count_t arg_count,sll_node_t*const* children,sll_node_t* parent){
	if (node->type!=SLL_NODE_TYPE_PRINT){
		return 0;
	}
	sll_bool_t update_arg_count=0;
	sll_node_t* prev=NULL;
	for (sll_arg_count_t i=0;i<arg_count;i++){
		sll_node_t* child=*(children+i);
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
				update_arg_count=1;
				child->type=SLL_NODE_TYPE_STRING;
				child->data.string_index=sll_add_string(&(source_file->string_table),&tmp);
			}
			prev=child;
		}
		else if (child->type==SLL_NODE_TYPE_STRING){
			sll_string_t* str=source_file->string_table.data+child->data.string_index;
			if (!str->length){
				child->type=SLL_NODE_TYPE_NOP;
				update_arg_count=1;
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
					update_arg_count=1;
					child->data.string_index=sll_add_string(&(source_file->string_table),&tmp);
				}
				else if (str->length==1){
					child->type=SLL_NODE_TYPE_CHAR;
					child->data.char_=str->data[0];
				}
				prev=child;
			}
		}
		else if (child->type!=SLL_NODE_TYPE_NOP){
			prev=NULL;
		}
	}
	return update_arg_count;
}

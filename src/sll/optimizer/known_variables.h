#include <sll/_internal/common.h>
#include <sll/_internal/optimizer.h>
#include <sll/_internal/static_object.h>
#include <sll/gc.h>
#include <sll/identifier.h>
#include <sll/memory.h>
#include <sll/node.h>
#include <sll/object.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/string_table.h>
#include <sll/types.h>



#define GET_VARIABLE_POINTER(identifier_index) (_variable_data[SLL_IDENTIFIER_GET_ARRAY_ID((identifier_index))]+SLL_IDENTIFIER_GET_ARRAY_INDEX((identifier_index)))



static sll_object_t** _variable_data[SLL_MAX_SHORT_IDENTIFIER_LENGTH+1];



static sll_object_t* _parse_value(const sll_source_file_t* source_file,const sll_node_t* node){
	switch (node->type){
		case SLL_NODE_TYPE_INT:
			return sll_int_to_object(node->data.int_);
		case SLL_NODE_TYPE_FLOAT:
			return sll_float_to_object(node->data.float_);
		case SLL_NODE_TYPE_CHAR:
			return SLL_FROM_CHAR(node->data.char_);
		case SLL_NODE_TYPE_COMPLEX:
			SLL_UNIMPLEMENTED();
		case SLL_NODE_TYPE_STRING:
			return STRING_TO_OBJECT(source_file->string_table.data+node->data.string_index);
		case SLL_NODE_TYPE_IDENTIFIER:
			{
				sll_object_t* value=*GET_VARIABLE_POINTER(node->data.identifier_index);
				if (value){
					SLL_ACQUIRE(value);
				}
				return value;
			}
	}
	return NULL;
}



// static void _build_value(sll_source_file_t* source_file,sll_object_t* object,sll_node_t* out){
// 	switch (object->type){
// 		case SLL_OBJECT_TYPE_INT:
// 			out->type=SLL_NODE_TYPE_INT;
// 			out->data.int_=object->data.int_;
// 			break;
// 		case SLL_OBJECT_TYPE_FLOAT:
// 			out->type=SLL_NODE_TYPE_FLOAT;
// 			out->data.float_=object->data.float_;
// 			break;
// 		case SLL_OBJECT_TYPE_CHAR:
// 			out->type=SLL_NODE_TYPE_CHAR;
// 			out->data.char_=object->data.char_;
// 			break;
// 		case SLL_OBJECT_TYPE_STRING:
// 			{
// 				sll_string_t tmp;
// 				sll_string_clone(&(object->data.string),&tmp);
// 				out->type=SLL_NODE_TYPE_STRING;
// 				out->data.string_index=sll_add_string(&(source_file->string_table),&tmp);
// 				break;
// 			}
// 		default:
// 			SLL_UNIMPLEMENTED();
// 	}
// }



static void _release_var_data(sll_object_t** data,sll_identifier_list_length_t length){
	while (length){
		length--;
		if (*(data+length)){
			SLL_RELEASE(*(data+length));
		}
	}
	sll_deallocate(data);
}



OPTIMIZER_FUNTION_INIT(known_variables){
	for (sll_identifier_index_t i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		_variable_data[i]=sll_zero_allocate(source_file->identifier_table.short_[i].length*sizeof(sll_object_t*));
	}
	_variable_data[SLL_MAX_SHORT_IDENTIFIER_LENGTH]=sll_zero_allocate(source_file->identifier_table.long_data_length*sizeof(sll_object_t*));
}



OPTIMIZER_FUNTION_DEINIT(known_variables){
	for (sll_identifier_index_t i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		_release_var_data(_variable_data[i],source_file->identifier_table.short_[i].length);
	}
	_release_var_data(_variable_data[SLL_MAX_SHORT_IDENTIFIER_LENGTH],source_file->identifier_table.long_data_length);
}



OPTIMIZER_FUNTION(known_variables){
	if (node->type==SLL_NODE_TYPE_IDENTIFIER&&parent&&parent->type!=SLL_NODE_TYPE_ASSIGN&&parent->type!=SLL_NODE_TYPE_ACCESS&&parent->type!=SLL_NODE_TYPE_VAR_ACCESS&&parent->type!=SLL_NODE_TYPE_INC&&parent->type!=SLL_NODE_TYPE_DEC){
		sll_object_t* value=*GET_VARIABLE_POINTER(node->data.identifier_index);
		if (value){
			// _build_value(source_file,value,node);
		}
	}
	else if (node->type==SLL_NODE_TYPE_ASSIGN){
		SLL_ASSERT(node->type>=2);
		sll_node_t* variable=*children;
		if (variable->type==SLL_NODE_TYPE_IDENTIFIER){
			sll_object_t** value=GET_VARIABLE_POINTER(variable->data.identifier_index);
			if (*value){
				SLL_RELEASE(*value);
			}
			*value=_parse_value(source_file,*(children+1));
		}
		else{
			// SLL_UNIMPLEMENTED();
		}
	}
	return node;
}

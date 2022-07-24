#include <sll/_internal/common.h>
#include <sll/assembly.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/location.h>
#include <sll/object.h>
#include <sll/string.h>
#include <sll/string_table.h>
#include <sll/types.h>
#include <sll/vm.h>



__SLL_EXTERNAL sll_file_offset_t sll_get_location(const sll_assembly_data_t* assembly_data,sll_instruction_index_t instruction_index,sll_string_index_t* file_path_string_index,sll_string_index_t* function_string_index){
	sll_string_index_t o_fp=(assembly_data->string_table.length?0:SLL_MAX_STRING_INDEX);
	if (instruction_index>=assembly_data->instruction_count){
		if (file_path_string_index){
			*file_path_string_index=o_fp;
		}
		if (function_string_index){
			*function_string_index=SLL_MAX_STRING_INDEX;
		}
		return 0;
	}
	sll_instruction_index_t c=0;
	sll_file_offset_t o_ln=0;
	sll_string_index_t o_fn=SLL_MAX_STRING_INDEX;
	for (sll_debug_data_length_t i=0;i<assembly_data->debug_data.length;i++){
		c+=(assembly_data->debug_data.data+i)->delta_instruction_index;
		if (c>instruction_index){
			break;
		}
		if ((assembly_data->debug_data.data+i)->line&SLL_DEBUG_LINE_DATA_FLAG_FILE){
			o_fp=(sll_string_index_t)SLL_DEBUG_LINE_DATA_GET_DATA(assembly_data->debug_data.data+i);
			o_ln=0;
		}
		else if ((assembly_data->debug_data.data+i)->line&SLL_DEBUG_LINE_DATA_FLAG_FUNC){
			o_fn=(sll_string_index_t)SLL_DEBUG_LINE_DATA_GET_DATA(assembly_data->debug_data.data+i);
		}
		else{
			o_ln=SLL_DEBUG_LINE_DATA_GET_DATA(assembly_data->debug_data.data+i);
		}
	}
	if (file_path_string_index){
		*file_path_string_index=o_fp;
	}
	if (function_string_index){
		*function_string_index=o_fn;
	}
	return o_ln+1;
}



__SLL_EXTERNAL void sll_get_name(sll_object_t object,sll_string_t* out){
	SLL_INIT_STRING(out);
	if (object->type==SLL_OBJECT_TYPE_INT){
		if (object->data.int_<0){
			sll_function_index_t i=(sll_function_index_t)(~object->data.int_);
			if (i<sll_current_runtime_data->internal_function_table->length){
				sll_string_clone(&((sll_current_runtime_data->internal_function_table->data+i)->name),out);
			}
		}
		else if (object->data.int_&&object->data.int_<=sll_current_runtime_data->assembly_data->function_table.length){
			sll_string_index_t nm_idx;
			sll_get_location(sll_current_runtime_data->assembly_data,(sll_current_runtime_data->assembly_data->function_table.data+object->data.int_-1)->instruction_index,NULL,&nm_idx);
			sll_string_clone(sll_current_runtime_data->assembly_data->string_table.data+nm_idx,out);
		}
	}
	else{
		sll_get_type_name(sll_current_runtime_data->type_table,object->type,out);
	}
}

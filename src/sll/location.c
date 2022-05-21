#include <sll/assembly.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/location.h>
#include <sll/object.h>
#include <sll/string.h>
#include <sll/string_table.h>
#include <sll/types.h>
#include <sll/vm.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_file_offset_t sll_get_location(const sll_assembly_data_t* a_dt,sll_instruction_index_t ii,sll_string_index_t* fp,sll_string_index_t* fn){
	if (ii>=a_dt->instruction_count){
		*fp=0;
		*fn=SLL_MAX_STRING_INDEX;
		return 0;
	}
	sll_instruction_index_t c=0;
	sll_file_offset_t o_ln=0;
	sll_string_index_t o_fn=SLL_MAX_STRING_INDEX;
	sll_string_index_t o_fp=0;
	for (sll_debug_data_length_t i=0;i<a_dt->debug_data.length;i++){
		c+=(a_dt->debug_data.data+i)->delta_instruction_index;
		if (c>ii){
			break;
		}
		if ((a_dt->debug_data.data+i)->line&SLL_DEBUG_LINE_DATA_FLAG_FILE){
			o_fp=(sll_string_index_t)SLL_DEBUG_LINE_DATA_GET_DATA(a_dt->debug_data.data+i);
			o_ln=0;
		}
		else if ((a_dt->debug_data.data+i)->line&SLL_DEBUG_LINE_DATA_FLAG_FUNC){
			o_fn=(sll_string_index_t)SLL_DEBUG_LINE_DATA_GET_DATA(a_dt->debug_data.data+i);
		}
		else{
			o_ln=SLL_DEBUG_LINE_DATA_GET_DATA(a_dt->debug_data.data+i);
		}
	}
	*fp=o_fp;
	*fn=o_fn;
	return o_ln+1;
}



__SLL_EXTERNAL void sll_get_name(sll_object_t* v,sll_string_t* o){
	SLL_INIT_STRING(o);
	if (v->type==SLL_OBJECT_TYPE_INT){
		if (v->data.int_<0){
			sll_function_index_t i=(sll_function_index_t)(~v->data.int_);
			if (i<sll_current_runtime_data->ift->l){
				sll_string_clone(&((sll_current_runtime_data->ift->dt+i)->nm),o);
			}
		}
		else if (v->data.int_&&v->data.int_<=sll_current_runtime_data->a_dt->function_table.length){
			sll_string_clone(sll_current_runtime_data->a_dt->string_table.dt+(sll_current_runtime_data->a_dt->function_table.data+v->data.int_-1)->name_string_index,o);
		}
	}
	else{
		sll_get_type_name(sll_current_runtime_data->tt,v->type,o);
	}
}

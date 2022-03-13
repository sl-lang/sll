#include <sll/common.h>
#include <sll/location.h>
#include <sll/object.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/vm.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_file_offset_t sll_get_location(const sll_assembly_data_t* a_dt,sll_instruction_index_t ii,sll_string_index_t* fp,sll_string_index_t* fn){
	if (ii>=a_dt->ic){
		*fp=0;
		*fn=SLL_MAX_STRING_INDEX;
		return 0;
	}
	sll_instruction_index_t c=0;
	sll_file_offset_t o_ln=0;
	sll_string_index_t o_fn=SLL_MAX_STRING_INDEX;
	sll_string_index_t o_fp=0;
	for (sll_instruction_index_t i=0;i<a_dt->dbg.l;i++){
		c+=(a_dt->dbg.dt+i)->ii;
		if (c>ii){
			break;
		}
		if ((a_dt->dbg.dt+i)->ln&SLL_DEBUG_LINE_DATA_FLAG_FILE){
			o_fp=(sll_string_index_t)SLL_DEBUG_LINE_DATA_GET_DATA(a_dt->dbg.dt+i);
			o_ln=0;
		}
		else if ((a_dt->dbg.dt+i)->ln&SLL_DEBUG_LINE_DATA_FLAG_FUNC){
			o_fn=(sll_string_index_t)SLL_DEBUG_LINE_DATA_GET_DATA(a_dt->dbg.dt+i);
		}
		else{
			o_ln=SLL_DEBUG_LINE_DATA_GET_DATA(a_dt->dbg.dt+i);
		}
	}
	*fp=o_fp;
	*fn=o_fn;
	return o_ln+1;
}



__SLL_EXTERNAL void sll_get_name(const sll_object_t* v,sll_string_t* o){
	SLL_INIT_STRING(o);
	if (SLL_OBJECT_GET_TYPE(v)==SLL_OBJECT_TYPE_INT){
		if (v->dt.i<0){
			sll_function_index_t i=(sll_function_index_t)(~v->dt.i);
			if (i<sll_current_runtime_data->ift->l){
				sll_string_clone(&((sll_current_runtime_data->ift->dt+i)->nm),o);
			}
		}
		else if (v->dt.i&&v->dt.i<=sll_current_runtime_data->a_dt->ft.l){
			sll_string_clone(sll_current_runtime_data->a_dt->st.dt+(sll_current_runtime_data->a_dt->ft.dt+v->dt.i-1)->nm,o);
		}
	}
	else if (SLL_OBJECT_GET_TYPE(v)>SLL_MAX_OBJECT_TYPE&&SLL_OBJECT_GET_TYPE(v)-SLL_MAX_OBJECT_TYPE-1<sll_current_runtime_data->tt->l){
		sll_string_clone(&((*(sll_current_runtime_data->tt->dt+SLL_OBJECT_GET_TYPE(v)-SLL_MAX_OBJECT_TYPE-1))->nm),o);
	}
}

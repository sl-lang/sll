#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/location.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_file_offset_t sll_get_location(const sll_assembly_data_t* a_dt,sll_instruction_index_t ii,sll_string_index_t* fp,sll_string_index_t* fn){
	if (ii>=a_dt->ic){
		*fp=0;
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

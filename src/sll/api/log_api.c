#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/api/string.h>
#include <sll/common.h>
#include <sll/log.h>
#include <sll/types.h>
#include <sll/util.h>
#include <sll/vm.h>



__API_FUNC(log_log){
	sll_string_index_t fp;
	sll_string_index_t fn;
	sll_file_offset_t ln=sll_get_location(sll_current_runtime_data->a_dt,sll_current_instruction_index,&fp,&fn);
	sll_string_t s;
	sll_api_string_convert(a,ac,&s);
	sll_log_raw((sll_current_runtime_data->a_dt->st.dt+fp)->v,(fn==SLL_MAX_STRING_INDEX?SLL_CHAR("@code"):(sll_current_runtime_data->a_dt->st.dt+fn)->v),ln,&s);
	sll_free_string(&s);
}



__API_FUNC(log_set_default){
	sll_set_log_default(a);
}



__API_FUNC(log_set_file){
	sll_set_log_file(a->v,b);
}



__API_FUNC(log_set_function){
	sll_set_log_function(a->v,b->v,c);
}

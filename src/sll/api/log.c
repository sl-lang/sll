#include <sll/api/string.h>
#include <sll/common.h>
#include <sll/location.h>
#include <sll/log.h>
#include <sll/thread.h>
#include <sll/types.h>
#include <sll/vm.h>



__SLL_EXTERNAL __SLL_API_CALL void sll_api_log_log(sll_object_t** a,sll_arg_count_t ac){
	sll_string_index_t fp;
	sll_string_index_t fn;
	sll_file_offset_t ln=sll_get_location(sll_current_runtime_data->a_dt,sll_thread_get_instruction_index(SLL_UNKNOWN_THREAD_INDEX),&fp,&fn);
	sll_string_t s;
	sll_api_string_convert(a,ac,&s);
	sll_log_raw((sll_current_runtime_data->a_dt->st.dt+fp)->v,(fn==SLL_MAX_STRING_INDEX?SLL_CHAR("@code"):(sll_current_runtime_data->a_dt->st.dt+fn)->v),ln,0,&s);
	sll_free_string(&s);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_log_set_default(sll_integer_t a,sll_bool_t b){
	sll_set_log_default((sll_flags_t)a,!!b);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_log_set_file(sll_string_t* a,sll_integer_t b,sll_bool_t c){
	sll_set_log_file(a->v,(sll_flags_t)b,!!c);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_log_set_function(sll_string_t* a,sll_string_t* b,sll_integer_t c,sll_bool_t d){
	sll_set_log_function(a->v,b->v,(sll_flags_t)c,!!d);
}

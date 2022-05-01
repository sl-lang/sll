#include <sll/api/string.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/location.h>
#include <sll/log.h>
#include <sll/string.h>
#include <sll/string_table.h>
#include <sll/thread.h>
#include <sll/types.h>
#include <sll/vm.h>



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_log_log(sll_object_t*const* data,sll_arg_count_t len){
	sll_string_index_t fp;
	sll_string_index_t fn;
	sll_file_offset_t ln=sll_get_location(sll_current_runtime_data->a_dt,sll_thread_get_instruction_index(SLL_UNKNOWN_THREAD_INDEX),&fp,&fn);
	sll_string_t s;
	sll_api_string_convert(data,len,&s);
	sll_bool_t o=sll_log_raw((sll_current_runtime_data->a_dt->st.dt+fp)->v,(fn==SLL_MAX_STRING_INDEX?SLL_CHAR("@code"):(sll_current_runtime_data->a_dt->st.dt+fn)->v),ln,0,&s);
	sll_free_string(&s);
	return o;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_log_set_default(sll_flags_t flags,sll_bool_t state){
	return sll_set_log_flags(NULL,NULL,flags,state);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_log_set_file(const sll_string_t* file,sll_flags_t flags,sll_bool_t state){
	return sll_set_log_flags(file->v,NULL,flags,state);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_log_set_function(const sll_string_t* file,const sll_string_t* func,sll_flags_t flags,sll_bool_t state){
	return sll_set_log_flags(file->v,func->v,flags,state);
}

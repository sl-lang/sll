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



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_log_log(const sll_object_t* args,sll_arg_count_t arg_count){
	sll_string_index_t fp;
	sll_string_index_t fn;
	sll_file_offset_t ln=sll_get_location(sll_current_runtime_data->assembly_data,sll_thread_get_instruction_index(SLL_UNKNOWN_THREAD_INDEX),&fp,&fn);
	sll_string_t s;
	sll_api_string_convert(args,arg_count,&s);
	sll_bool_t o=sll_log_raw((fp==SLL_MAX_STRING_INDEX?SLL_CHAR("@console"):(sll_current_runtime_data->assembly_data->string_table.data+fp)->data),(fn==SLL_MAX_STRING_INDEX?SLL_CHAR("@code"):(sll_current_runtime_data->assembly_data->string_table.data+fn)->data),ln,0,&s);
	sll_free_string(&s);
	return o;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_log_set(const sll_string_t* file,const sll_string_t* func,sll_logger_flags_t flags,sll_bool_t state){
	return sll_set_log_flags((file?file->data:NULL),(func?func->data:NULL),flags,state);
}

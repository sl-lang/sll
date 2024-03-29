#include <sll/_internal/common.h>
#include <sll/audit.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/new_object.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/var_arg.h>



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_object_t sll_api_object_new(const sll_string_t* format,const sll_object_t* args,sll_arg_count_t length){
	sll_var_arg_list_t va_list;
	SLL_VAR_ARG_INIT_SLL(&va_list,args,length);
	sll_audit(SLL_CHAR("sll.object.new"),SLL_CHAR("sO+"),format,args,length);
	return sll_new_object_list(format->data,format->length,&va_list);
}

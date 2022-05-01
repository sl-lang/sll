#include <sll/_internal/common.h>
#include <sll/audit.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/new_object.h>
#include <sll/types.h>
#include <sll/var_arg.h>



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_object_t* sll_api_object_new(const sll_string_t* fmt,sll_object_t*const* args,sll_arg_count_t len){
	sll_var_arg_list_t dt;
	SLL_VAR_ARG_INIT_SLL(&dt,args,len);
	sll_audit(SLL_CHAR("sll.object.new"),SLL_CHAR("sO+"),fmt,args,len);
	return sll_new_object_list(fmt->v,fmt->l,&dt);
}

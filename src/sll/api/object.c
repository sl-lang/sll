#include <sll/_internal/common.h>
#include <sll/audit.h>
#include <sll/common.h>
#include <sll/new_object.h>
#include <sll/types.h>
#include <sll/var_arg.h>



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_object_t* sll_api_object_new(sll_string_t* a,sll_object_t*const* b,sll_arg_count_t bc){
	sll_var_arg_list_t dt={
		SLL_VAR_ARG_LIST_TYPE_SLL,
		{
			.sll={
				b,
				bc
			}
		}
	};
	sll_audit(SLL_CHAR("sll.object.new"),SLL_CHAR("sO+"),a,b,bc);
	return sll_new_object_list(a->v,a->l,&dt);
}

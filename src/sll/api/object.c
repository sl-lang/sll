#include <sll/_internal/api.h>
#include <sll/_internal/common.h>
#include <sll/api.h>
#include <sll/audit.h>
#include <sll/common.h>
#include <sll/new_object.h>
#include <sll/types.h>
#include <sll/var_arg.h>



__API_FUNC(object_new){
	sll_var_arg_list_t dt={
		SLL_VAR_ARG_LIST_TYPE_SLL,
		{
			.sll={
				b,
				bc
			}
		}
	};
	sll_audit(SLL_CHAR("sll.object.new"),SLL_CHAR("sO*"),a,b,bc);
	return sll_new_object_list(a->v,a->l,&dt);
}

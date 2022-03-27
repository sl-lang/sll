#include <sll/_internal/api.h>
#include <sll/_internal/common.h>
#include <sll/api.h>
#include <sll/audit.h>
#include <sll/common.h>
#include <sll/types.h>
#include <sll/var_arg.h>



__API_FUNC(audit_audit){
	sll_var_arg_list_t dt={
		SLL_VAR_ARG_LIST_TYPE_SLL,
		{
			.sll={
				c,
				cc
			}
		}
	};
	sll_audit_list(a->v,b->v,&dt);
}



__API_FUNC(audit_register_callback){
	SLL_UNIMPLEMENTED();
}



__API_FUNC(audit_unregister_callback){
	SLL_UNIMPLEMENTED();
}

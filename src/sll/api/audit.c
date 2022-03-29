#include <sll/_internal/api.h>
#include <sll/_internal/common.h>
#include <sll/_internal/vm.h>
#include <sll/api.h>
#include <sll/audit.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/init.h>
#include <sll/static_object.h>
#include <sll/memory.h>
#include <sll/types.h>
#include <sll/var_arg.h>
#include <sll/vm.h>



static sll_integer_t _audit_cb=0;



static void _call_user_cb(const sll_string_t* nm,const sll_array_t* arg){
	sll_object_t* dt[]={
		sll_string_to_object(nm),
		sll_array_to_object(arg)
	};
	sll_release_object(sll_execute_function(_audit_cb,dt,2,EXECUTE_FUNCTION_NO_AUDIT_TERMINATE));
	sll_release_object(dt[0]);
	sll_release_object(dt[1]);
}



void _audit_cleanup_api(void){
	_audit_cb=0;
}



__API_FUNC(audit__init){
	if (!a){
		return;
	}
	if (!_audit_cb){
		sll_audit_register_callback(_call_user_cb);
	}
	_audit_cb=a;
}



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

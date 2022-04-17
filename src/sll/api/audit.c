#include <sll/_internal/common.h>
#include <sll/_internal/gc.h>
#include <sll/_internal/static_object.h>
#include <sll/_internal/vm.h>
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
	if (!_audit_cb){
		return;
	}
	sll_object_t* dt[]={
		STRING_TO_OBJECT(nm),
		sll_array_to_object(arg)
	};
	GC_RELEASE(sll_execute_function(_audit_cb,dt,2,EXECUTE_FUNCTION_NO_AUDIT_TERMINATE));
	GC_RELEASE(dt[0]);
	GC_RELEASE(dt[1]);
}



void _audit_cleanup_api(void){
	_audit_cb=0;
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_audit__init(sll_integer_t a){
	if (!a){
		return;
	}
	if (!_audit_cb){
		sll_audit_register_callback(_call_user_cb);
	}
	_audit_cb=a;
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_audit_audit(sll_string_t* a,sll_string_t* b,sll_object_t*const* c,sll_arg_count_t cc){
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

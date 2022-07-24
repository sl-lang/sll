#include <sll/_internal/common.h>
#include <sll/_internal/static_object.h>
#include <sll/_internal/vm.h>
#include <sll/audit.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/init.h>
#include <sll/memory.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/var_arg.h>
#include <sll/vm.h>



static sll_integer_t _audit_callback=0;



static void _call_user_callback(const sll_string_t* name,const sll_array_t* args){
	if (!_audit_callback){
		return;
	}
	sll_object_t dt[2]={
		STRING_TO_OBJECT(name),
		sll_array_to_object(args)
	};
	sll_gc_add_roots(dt,2);
	SLL_RELEASE(sll_execute_function(_audit_callback,dt,2,EXECUTE_FUNCTION_NO_AUDIT_TERMINATE));
	sll_gc_remove_roots(dt);
	SLL_RELEASE(dt[0]);
	SLL_RELEASE(dt[1]);
}



static void _cleanup_data(void){
	sll_audit_unregister_callback(_call_user_callback);
	_audit_callback=0;
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_audit__init(sll_integer_t callback){
	if (!callback){
		return;
	}
	if (!_audit_callback){
		sll_audit_register_callback(_call_user_callback);
		sll_register_cleanup(_cleanup_data,SLL_CLEANUP_TYPE_VM);
	}
	_audit_callback=callback;
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_audit_audit(const sll_string_t* name,const sll_string_t* format,const sll_object_t* args,sll_arg_count_t arg_count){
	sll_var_arg_list_t dt;
	SLL_VAR_ARG_INIT_SLL(&dt,args,arg_count);
	sll_audit_list(name->data,format->data,&dt);
}

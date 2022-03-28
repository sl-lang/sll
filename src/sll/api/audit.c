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



static sll_integer_t* _audit_cb=NULL;
static sll_array_length_t _audit_cb_len=0;
static sll_bool_t _audit_cb_registered=0;



static void _cleanup_data(void){
	_audit_cb_registered=0;
}



static void _call_user_cb(const sll_string_t* nm,const sll_array_t* arg){
	sll_object_t* dt[]={
		sll_string_to_object(nm),
		sll_array_to_object(arg)
	};
	for (sll_array_length_t i=0;i<_audit_cb_len;i++){
		sll_release_object(sll_execute_function(*(_audit_cb+i),dt,2,EXECUTE_FUNCTION_NO_AUDIT_TERMINATE));
	}
	sll_release_object(dt[0]);
	sll_release_object(dt[1]);
}



void _audit_cleanup_api(void){
	sll_deallocate(_audit_cb);
	_audit_cb=NULL;
	_audit_cb_len=0;
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



__API_FUNC(audit_register){
	if (!a){
		return;
	}
	if (!_audit_cb_registered){
		_audit_cb_registered=1;
		sll_audit_register_callback(_call_user_cb);
		sll_register_cleanup(_cleanup_data);
	}
	_audit_cb_len++;
	_audit_cb=sll_reallocate(_audit_cb,_audit_cb_len*sizeof(sll_audit_callback_t));
	*(_audit_cb+_audit_cb_len-1)=a;
}



__API_FUNC(audit_unregister){
	if (!a){
		return 0;
	}
	sll_array_length_t i=0;
	sll_array_length_t j=0;
	for (;j<_audit_cb_len;j++){
		if (*(_audit_cb+j)!=a){
			*(_audit_cb+i)=*(_audit_cb+j);
			i++;
		}
	}
	if (i==j){
		return 0;
	}
	_audit_cb_len=i;
	_audit_cb=sll_reallocate(_audit_cb,_audit_cb_len*sizeof(sll_integer_t));
	return 1;
}

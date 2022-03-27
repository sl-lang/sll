#include <sll/_internal/common.h>
#include <sll/audit.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/new_object.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/var_arg.h>
#include <stdarg.h>



static sll_array_length_t _audit_cb_len=0;



__SLL_EXTERNAL void sll_audit(const sll_char_t* nm,const sll_char_t* t,...){
	if (!_audit_cb_len){
		return;
	}
	va_list va;
	va_start(va,t);
	sll_var_arg_list_t dt={
		SLL_VAR_ARG_LIST_TYPE_C,
		{
			.c=&va
		}
	};
	sll_audit_list(nm,t,&dt);
	va_end(va);
}



__SLL_EXTERNAL void sll_audit_list(const sll_char_t* nm,const sll_char_t* t,sll_var_arg_list_t* va){
	if (!_audit_cb_len){
		return;
	}
	sll_object_t* o=sll_new_object_list(t,sll_string_length_unaligned(t),va);
	for (sll_array_length_t i=0;i<_audit_cb_len;i++){
		SLL_UNIMPLEMENTED();
	}
	sll_release_object(o);
}


#include <sll/_internal/common.h>
#include <sll/array.h>
#include <sll/audit.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/memory.h>
#include <sll/new_object.h>
#include <sll/object.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/var_arg.h>
#include <stdarg.h>



static sll_audit_callback_t* _audit_cb=NULL;
static sll_array_length_t _audit_cb_len=0;



void _audit_cleanup(void){
	sll_deallocate(_audit_cb);
	_audit_cb=NULL;
	_audit_cb_len=0;
}



__SLL_EXTERNAL void sll_audit(const sll_char_t* nm,const sll_char_t* t,...){
	va_list va;
	va_start(va,t);
	sll_var_arg_list_t dt;
	SLL_VAR_ARG_INIT_C(&dt,&va);
	sll_audit_list(nm,t,&dt);
	va_end(va);
}



__SLL_EXTERNAL void sll_audit_list(const sll_char_t* nm,const sll_char_t* t,sll_var_arg_list_t* va){
	sll_string_t nm_s;
	sll_string_from_pointer(nm,&nm_s);
	sll_array_t arr;
	sll_new_object_array_list(t,sll_string_length(t),va,&arr);
	for (sll_array_length_t i=0;i<_audit_cb_len;i++){
		(*(_audit_cb+i))(&nm_s,&arr);
	}
	sll_free_array(&arr);
	sll_free_string(&nm_s);
}



__SLL_EXTERNAL void sll_audit_register_callback(sll_audit_callback_t fn){
	_audit_cb_len++;
	_audit_cb=sll_reallocate(_audit_cb,_audit_cb_len*sizeof(sll_audit_callback_t));
	*(_audit_cb+_audit_cb_len-1)=fn;
}



__SLL_EXTERNAL sll_bool_t sll_audit_unregister_callback(sll_audit_callback_t fn){
	sll_array_length_t i=0;
	sll_array_length_t j=0;
	for (;j<_audit_cb_len;j++){
		if (*(_audit_cb+j)!=fn){
			*(_audit_cb+i)=*(_audit_cb+j);
			i++;
		}
	}
	if (i==j){
		return 0;
	}
	_audit_cb_len=i;
	_audit_cb=sll_reallocate(_audit_cb,_audit_cb_len*sizeof(sll_audit_callback_t));
	return 1;
}

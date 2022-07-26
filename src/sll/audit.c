#include <sll/_internal/common.h>
#include <sll/array.h>
#include <sll/audit.h>
#include <sll/common.h>
#include <sll/container.h>
#include <sll/gc.h>
#include <sll/memory.h>
#include <sll/new_object.h>
#include <sll/object.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/var_arg.h>
#include <stdarg.h>



static sll_container_t _audit_data=SLL_CONTAINER_INIT_STRUCT;
static sll_bool_t _audit_enable=1;



void _audit_cleanup(void){
	SLL_CONTAINER_CLEAR(&_audit_data);
	_audit_enable=1;
}



__SLL_EXTERNAL void sll_audit(const sll_char_t* name,const sll_char_t* format,...){
	if (!_audit_enable){
		return;
	}
	va_list va;
	va_start(va,format);
	sll_var_arg_list_t dt;
	SLL_VAR_ARG_INIT_C(&dt,&va);
	sll_audit_list(name,format,&dt);
	va_end(va);
}



__SLL_EXTERNAL sll_bool_t sll_audit_enable(sll_bool_t enable){
	sll_bool_t out=_audit_enable;
	_audit_enable=enable;
	return out;
}



__SLL_EXTERNAL void sll_audit_list(const sll_char_t* name,const sll_char_t* format,sll_var_arg_list_t* va){
	if (!_audit_enable||!name){
		return;
	}
	sll_string_t name_str;
	sll_string_from_pointer(name,&name_str);
	sll_array_t args;
	sll_new_object_array_list(format,sll_string_length(format),va,&args);
	SLL_CONTAINER_ITER(&_audit_data,sll_audit_callback_t,callback,{
		callback(&name_str,&args);
	});
	sll_free_array(&args);
	sll_free_string(&name_str);
}



__SLL_EXTERNAL void sll_audit_register_callback(sll_audit_callback_t callback){
	SLL_CONTAINER_PUSH(&_audit_data,sll_audit_callback_t,callback);
}



__SLL_EXTERNAL sll_bool_t sll_audit_unregister_callback(sll_audit_callback_t callback){
	sll_bool_t out=0;
	SLL_CONTAINER_FILTER(&_audit_data,sll_audit_callback_t,elem,elem!=callback,{
		out=1;
	});
	return out;
}

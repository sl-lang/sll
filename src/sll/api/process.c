#include <sll/_internal/common.h>
#include <sll/_internal/static_string.h>
#include <sll/_internal/string.h>
#include <sll/allocator.h>
#include <sll/api/process.h>
#include <sll/array.h>
#include <sll/audit.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/environment.h>
#include <sll/error.h>
#include <sll/gc.h>
#include <sll/memory.h>
#include <sll/new_object.h>
#include <sll/object.h>
#include <sll/operator.h>
#include <sll/platform/process.h>
#include <sll/sandbox.h>
#include <sll/search_path.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/vm.h>



static const bitmap_t _process_quote_chars[4]={
	0xd80087dfffffffffull,// %+,-.0123456789:=
	0xf800000178000000ull,// @ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz
	0xffffffffffffffffull,
	0xffffffffffffffffull
};



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_process_execute_shell(const sll_string_t* cmd){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_PROCESS_API)){
		return SLL_ERROR_FROM_SANDBOX(SLL_SANDBOX_FLAG_DISABLE_PROCESS_API);
	}
	sll_audit(SLL_CHAR("sll.process.start.shell"),SLL_CHAR("s"),cmd);
	sll_error_t err;
	return (sll_platform_execute_shell(cmd->data,&err)?SLL_NO_ERROR:err);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_pid_t sll_api_process_get_pid(void){
	return (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_PROCESS_API)?0:sll_platform_get_pid());
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_process_join(const sll_string_t*const* args,sll_arg_count_t arg_count,sll_string_t* out){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_PROCESS_API)||!arg_count){
		SLL_INIT_STRING(out);
		return;
	}
	sll_char_t** dt=sll_allocate_stack(arg_count*sizeof(sll_char_t*));
	for (sll_array_length_t i=0;i<arg_count;i++){
		const sll_string_t* str=*(args+i);
		sll_char_t* p=sll_allocate_stack(str->length);
		sll_copy_data(str->data,str->length,p);
		*(dt+i)=p;
	}
	sll_process_join_args((const sll_char_t*const*)dt,out);
	for (sll_array_length_t i=0;i<arg_count;i++){
		sll_deallocate(*(dt+i));
	}
	sll_deallocate(dt);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_process_split(const sll_string_t* args,sll_array_t* out){
	if (!args){
		SLL_INIT_ARRAY(out);
		return;
	}
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_process_start(const sll_array_t* args,const sll_string_t* cwd,sll_process_creation_flags_t flags,const sll_string_t* stdin,sll_array_t* out){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_PROCESS_API)||!args->length){
		sll_new_object_array(SLL_CHAR("a(sh)0(sZZ)"),out,args,cwd,flags,stdin);
		return;
	}
	sll_audit(SLL_CHAR("sll.process.start"),SLL_CHAR("ass"),args,cwd,stdin);
	sll_object_t* n=sll_operator_cast(args->data[0],sll_static_int[SLL_OBJECT_TYPE_STRING]);
	sll_string_t exe_fp;
	if (!sll_search_path_find(sll_environment_path,&(n->data.string),SLL_SEARCH_PATH_FLAG_AFTER,&exe_fp)){
		SLL_UNIMPLEMENTED();
	}
	SLL_RELEASE(n);
	sll_char_t** raw_args=sll_allocate((args->length+1)*sizeof(sll_char_t*));
	*raw_args=sll_allocate((exe_fp.length+1)*sizeof(sll_char_t));
	sll_copy_data(exe_fp.data,exe_fp.length+1,*raw_args);
	sll_free_string(&exe_fp);
	for (sll_array_length_t i=1;i<args->length;i++){
		n=sll_operator_cast(args->data[i],sll_static_int[SLL_OBJECT_TYPE_STRING]);
		sll_bool_t trailing_slash=(n->data.string.data[n->data.string.length-1]=='\\');
		sll_char_t* ptr=sll_allocate((n->data.string.length+trailing_slash+1)*sizeof(sll_char_t));
		sll_copy_data(n->data.string.data,n->data.string.length+1,ptr);
		if (trailing_slash){
			*(ptr+n->data.string.length)='\\';
			*(ptr+n->data.string.length+1)=0;
		}
		*(raw_args+i)=ptr;
		SLL_RELEASE(n);
	}
	*(raw_args+args->length)=NULL;
	sll_error_t err;
	sll_process_handle_t ph=sll_platform_start_process((const sll_char_t*const*)raw_args,NULL,&err);
	if (!ph){
		SLL_UNIMPLEMENTED();
	}
	if (!(flags&SLL_PROCESS_FLAG_WAIT)){
		SLL_UNIMPLEMENTED();
	}
	sll_return_code_t rc;
	do{
		rc=sll_platform_wait_process(ph,&err);
	} while (rc!=SLL_NO_ERROR&&SLL_ERROR_GET_TYPE(err)==SLL_ERROR_FLAG_SYSTEM&&SLL_ERROR_GET_VALUE(err)==4);
	if (err!=SLL_NO_ERROR){
		SLL_UNIMPLEMENTED();
	}
	SLL_CRITICAL(sll_platform_close_process_handle(ph)==SLL_NO_ERROR);
	for (sll_array_length_t i=0;i<args->length;i++){
		sll_deallocate(*(raw_args+i));
	}
	sll_deallocate(raw_args);
	sll_new_object_array(SLL_CHAR("a(sh)h(sZZ)"),out,args,cwd,flags,rc,stdin);
}



__SLL_EXTERNAL void sll_process_join_args(const sll_char_t*const* a,sll_string_t* o){
	STRING_INIT_STACK(o);
	SLL_ASSERT(*a);
	do{
		if (o->length){
			sll_string_increase(o,1);
			o->data[o->length]=' ';
			o->length++;
		}
		sll_string_t tmp;
		sll_string_from_pointer(*a,&tmp);
		a++;
		if (!tmp.length){
			sll_string_increase(o,2);
			o->data[o->length]='"';
			o->data[o->length+1]='"';
			o->length+=2;
			goto _continue;
		}
		for (sll_string_length_t i=0;i<tmp.length;i++){
			if (_process_quote_chars[tmp.data[i]>>6]&(1ull<<(tmp.data[i]&63))){
				goto _quote;
			}
		}
		sll_string_increase(o,tmp.length);
		sll_copy_data(tmp.data,tmp.length,o->data+o->length);
		o->length+=tmp.length;
		goto _continue;
_quote:
		sll_string_increase(o,1);
		o->data[o->length]='"';
		o->length++;
		for (sll_string_length_t i=0;i<tmp.length;i++){
			if (tmp.data[i]=='"'){
				sll_string_increase(o,3);
				o->data[o->length]='"';
				o->data[o->length+1]='"';
				o->data[o->length+2]='"';
				o->length+=3;
			}
			else{
				sll_string_increase(o,1);
				o->data[o->length]=tmp.data[i];
				o->length++;
			}
		}
		sll_string_increase(o,1);
		o->data[o->length]='"';
		o->length++;
_continue:
		sll_free_string(&tmp);
	} while (*a);
	sll_allocator_move((void**)(&(o->data)),SLL_MEMORY_MOVE_DIRECTION_FROM_STACK);
}

#include <windows.h>
#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/memory.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/util.h>
#include <stdint.h>



static const uint64_t _win_process_quote_chars[4]={
	0xd80087dfffffffff,// %+,-.0123456789:=
	0xf800000178000000,// @ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz
	0xffffffffffffffff,
	0xffffffffffffffff
};



static void _join_args(const sll_char_t*const* a,sll_string_t* o){
	sll_string_create(0,o);
	o->v=sll_memory_move(o->v,SLL_MEMORY_MOVE_DIRECTION_TO_STACK);
	SLL_ASSERT(*a);
	do{
		if (o->l){
			sll_string_increase(o,1);
			o->v[o->l]=' ';
			o->l++;
		}
		sll_string_t tmp;
		sll_string_from_pointer(*a,&tmp);
		a++;
		if (!tmp.l){
			sll_string_increase(o,2);
			o->v[o->l]='\'';
			o->v[o->l+1]='\'';
			o->l+=2;
			goto _continue;
		}
		for (sll_string_length_t i=0;i<tmp.l;i++){
			if (_win_process_quote_chars[tmp.v[i]>>6]&(1ull<<(tmp.v[i]&63))){
				goto _quote;
			}
		}
		sll_string_increase(o,tmp.l);
		sll_copy_data(tmp.v,tmp.l,o->v+o->l);
		o->l+=tmp.l;
		goto _continue;
_quote:
		sll_string_increase(o,1);
		o->v[o->l]='\'';
		o->l++;
		for (sll_string_length_t i=0;i<tmp.l;i++){
			if (tmp.v[i]=='\''){
				sll_string_increase(o,5);
				o->v[o->l]='\'';
				o->v[o->l+1]='"';
				o->v[o->l+2]='\'';
				o->v[o->l+3]='"';
				o->v[o->l+4]='\'';
				o->l+=5;
			}
			else{
				sll_string_increase(o,1);
				o->v[o->l]=tmp.v[i];
				o->l++;
			}
		}
		sll_string_increase(o,1);
		o->v[o->l]='\'';
		o->l++;
_continue:
		sll_free_string(&tmp);
	} while (*a);
	o->v=sll_memory_move(o->v,SLL_MEMORY_MOVE_DIRECTION_FROM_STACK);
}



__SLL_EXTERNAL void sll_platform_close_process_handle(sll_process_handle_t ph){
	CloseHandle(ph);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_execute_shell(const sll_char_t* cmd){
	sll_string_t s;
	sll_string_format(SLL_CHAR("C:\\Windows\\System32\\cmd.exe /q /c %s"),&s,cmd);
	STARTUPINFOA si={0};
	si.cb=sizeof(STARTUPINFOA);
	si.dwFlags=0;
	PROCESS_INFORMATION pi;
	if (!CreateProcessA("C:\\Windows\\System32\\cmd.exe",s.v,NULL,NULL,TRUE,CREATE_NEW_PROCESS_GROUP,NULL,NULL,&si,&pi)){
		sll_free_string(&s);
		return -1;
	}
	sll_free_string(&s);
	CloseHandle(pi.hThread);
	DWORD ec;
	if (WaitForSingleObject(pi.hProcess,INFINITE)!=WAIT_OBJECT_0||!GetExitCodeProcess(pi.hProcess,&ec)){
		return 0;
	}
	CloseHandle(pi.hProcess);
	return !!ec;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_pid_t sll_platform_get_pid(void){
	return GetCurrentProcessId();
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_process_handle_t sll_platform_start_process(const sll_char_t*const* a,const sll_char_t*const* env){
	if (!a||!(*a)){
		return 0;
	}
	sll_string_t s;
	_join_args(a,&s);
	STARTUPINFOA si={0};
	si.cb=sizeof(STARTUPINFOA);
	si.dwFlags=0;
	PROCESS_INFORMATION pi;
	if (!CreateProcessA((char*)(*a),s.v,NULL,NULL,TRUE,CREATE_NEW_PROCESS_GROUP,(void*)env,NULL,&si,&pi)){
		sll_free_string(&s);
		return 0;
	}
	sll_free_string(&s);
	CloseHandle(pi.hThread);
	return pi.hProcess;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_return_code_t sll_platform_wait_process(sll_process_handle_t ph){
	DWORD ec;
	if (WaitForSingleObject(ph,INFINITE)!=WAIT_OBJECT_0||!GetExitCodeProcess(ph,&ec)){
		return -1;
	}
	return ec;
}


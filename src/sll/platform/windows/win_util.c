#include <windows.h>
#include <bcrypt.h>
#include <sll/common.h>
#include <sll/memory.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/util.h>
#include <signal.h>
#include <stdint.h>



static HANDLE _win_wh=INVALID_HANDLE_VALUE;
static sll_environment_t _win_env={NULL,0};



void* _win_dll_handle=NULL;



__SLL_EXTERNAL const sll_char_t* sll_platform_string=SLL_CHAR("windows");
__SLL_EXTERNAL const sll_environment_t* sll_environment=&_win_env;



static void _cleanup_env_data(void){
	for (sll_array_length_t i=0;i<_win_env.l;i++){
		const sll_environment_variable_t* kv=*(_win_env.dt+i);
		sll_free_string((sll_string_t*)(&(kv->k)));
		sll_free_string((sll_string_t*)(&(kv->v)));
		sll_deallocate((void*)kv);
	}
	*((sll_array_length_t*)(&(_win_env.l)))=0;
	sll_deallocate((void*)(_win_env.dt));
	_win_env.dt=NULL;
}



static void _release_handle(void){
	CloseHandle(_win_wh);
	_win_wh=INVALID_HANDLE_VALUE;
}



void _init_platform(void){
	LPCH dt=GetEnvironmentStrings();
	sll_array_length_t l=0;
	sll_environment_variable_t** kv=sll_allocate_stack(1);
	LPCH p=dt;
	while (*p){
		LPCH e=p;
		if (*p=='='){
			p+=sll_string_length_unaligned(p)+1;
			continue;
		}
		while (*p&&*p!='='){
			p++;
		}
		if (!(*p)){
			continue;
		}
		l++;
		kv=sll_reallocate(kv,l*sizeof(sll_environment_variable_t*));
		sll_environment_variable_t* n=sll_allocate(sizeof(sll_environment_variable_t));
		sll_string_from_pointer_length(e,(sll_string_length_t)(p-e),(sll_string_t*)(&(n->k)));
		p++;
		sll_string_length_t i=sll_string_length_unaligned(p);
		sll_string_from_pointer_length(p,i,(sll_string_t*)(&(n->v)));
		p+=i+1;
		*(kv+l-1)=n;
	}
	FreeEnvironmentStringsA(dt);
	if (!l){
		_win_env.dt=NULL;
	}
	else{
		_win_env.dt=sll_memory_move(kv,SLL_MEMORY_MOVE_DIRECTION_FROM_STACK);
	}
	*((sll_array_length_t*)(&(_win_env.l)))=l;
	sll_register_cleanup(_cleanup_env_data);
}



__SLL_NO_RETURN void _force_exit_platform(void){
	TerminateProcess(GetCurrentProcess(),SIGABRT);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_time_t sll_platform_get_current_time(void){
	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);
	return ((((sll_time_t)ft.dwHighDateTime)<<32)|ft.dwLowDateTime)*100-11644473600000000000;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_size_t sll_platform_get_cpu_count(void){
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	return si.dwNumberOfProcessors;
}



__SLL_EXTERNAL void sll_platform_random(void* bf,sll_size_t l){
	while (l){
		ULONG n=(l>UINT32_MAX?UINT32_MAX:(ULONG)l);
		if (BCryptGenRandom(NULL,bf,n,BCRYPT_USE_SYSTEM_PREFERRED_RNG)<0){
			sll_set_memory(bf,l,0);
		}
		l-=n;
		bf=(void*)(((uint64_t)bf)+n);
	}
}



__SLL_EXTERNAL void sll_platform_sleep(sll_time_t tm){
	if (_win_wh==INVALID_HANDLE_VALUE){
		_win_wh=CreateEventA(NULL,TRUE,FALSE,FALSE);
		sll_register_cleanup(_release_handle);
	}
	sll_time_t e=GetTickCount64()*1000000+tm;
	while (1){
		ResetEvent(_win_wh);
		if (WaitForSingleObjectEx(_win_wh,(DWORD)((tm+999999)/1000000),FALSE)!=WAIT_OBJECT_0){
			return;
		}
		sll_time_t c=GetTickCount64()*1000000;
		if (c>=e){
			break;
		}
		tm=c-e;
	}
}



BOOL WINAPI DllMain(HINSTANCE dll_h,DWORD t,LPVOID r){
	if (!_win_dll_handle){
		_win_dll_handle=dll_h;
	}
	return TRUE;
}

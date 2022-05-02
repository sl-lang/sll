#include <windows.h>
#include <sll/_internal/common.h>
#include <sll/common.h>
#include <sll/environment.h>
#include <sll/memory.h>
#include <sll/string.h>
#include <sll/types.h>



void _init_platform_env(void){
	LPCH dt=GetEnvironmentStrings();
	sll_environment_length_t l=0;
	sll_environment_variable_t** kv=sll_allocate_stack(1);
	LPCH p=dt;
	while (*p){
		LPCH e=p;
		if (*p=='='){
			p+=sll_string_length(p)+1;
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
		sll_string_length_t i=sll_string_length(p);
		sll_string_from_pointer_length(p,i,(sll_string_t*)(&(n->v)));
		p+=i+1;
		*(kv+l-1)=n;
	}
	FreeEnvironmentStringsA(dt);
	if (!l){
		sll_deallocate(kv);
		kv=NULL;
	}
	else{
		kv=sll_memory_move(kv,SLL_MEMORY_MOVE_DIRECTION_FROM_STACK);
	}
	*((void**)(&(sll_environment->dt)))=kv;
	*((sll_environment_length_t*)(&(sll_environment->l)))=l;
}



__SLL_EXTERNAL void sll_platform_remove_environment_variable(const sll_char_t* k){
	SetEnvironmentVariable((char*)k,NULL);
}



__SLL_EXTERNAL void sll_platform_set_environment_variable(const sll_char_t* k,const sll_char_t* v){
	SetEnvironmentVariable((char*)k,(char*)v);
}

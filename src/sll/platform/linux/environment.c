#include <sll/_internal/common.h>
#include <sll/common.h>
#include <sll/environment.h>
#include <sll/memory.h>
#include <sll/string.h>
#include <sll/types.h>
#include <stdlib.h>



extern char** environ;



void _init_platform_env(void){
	sll_environment_length_t l=0;
	char** dt=environ;
	while (*dt){
		l++;
		dt++;
	}
	*((void**)(&(sll_environment->dt)))=sll_allocate(l*sizeof(sll_environment_variable_t*));
	sll_environment_length_t i=0;
	dt=environ;
	while (*dt){
		sll_char_t* e=SLL_CHAR(*dt);
		dt++;
		if (*e=='='){
			continue;
		}
		sll_string_length_t j=0;
		while (*e&&*e!='='){
			j++;
			e++;
		}
		if (!(*e)){
			continue;
		}
		sll_environment_variable_t* n=sll_allocate(sizeof(sll_environment_variable_t));
		sll_string_from_pointer_length(e-j,j,(sll_string_t*)(&(n->k)));
		sll_string_from_pointer(e+1,(sll_string_t*)(&(n->v)));
		*(((const sll_environment_variable_t**)(sll_environment->dt))+i)=n;
		i++;
	}
	if (i!=l){
		l=i;
		*((void**)(&(sll_environment->dt)))=sll_reallocate((const sll_environment_variable_t**)(sll_environment->dt),l*sizeof(sll_environment_variable_t*));
	}
	*((sll_environment_length_t*)(&(sll_environment->l)))=l;
}



__SLL_EXTERNAL void sll_platform_remove_environment_variable(const sll_char_t* k){
	unsetenv((char*)k);
}



__SLL_EXTERNAL void sll_platform_set_environment_variable(const sll_char_t* k,const sll_char_t* v){
	setenv((char*)k,(char*)v,1);
}

#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/constants.h>
#include <sll/gc.h>
#include <sll/platform.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>
#include <stdlib.h>
#include <string.h>



static sll_runtime_object_t _sys_argc={1,SLL_RUNTIME_OBJECT_TYPE_INT,.dt={.i=0}};
static sll_runtime_object_t** _sys_argv=NULL;
static sll_runtime_object_t _sys_p={1,SLL_RUNTIME_OBJECT_TYPE_STRING,.dt={.s=SLL_ZERO_STRING_STRUCT}};



void _sys_free_argv(void){
	if (_sys_argv){
		for (sll_integer_t i=0;i<_sys_argc.dt.i;i++){
			SLL_RELEASE(*(_sys_argv+i));
		}
		free(_sys_argv);
		_sys_argv=NULL;
	}
}



__SLL_FUNC void sll_set_argument_count(sll_integer_t ac){
	if (_sys_argv){
		for (sll_integer_t i=0;i<_sys_argc.dt.i;i++){
			SLL_RELEASE(*(_sys_argv+i));
		}
		free(_sys_argv);
	}
	_sys_argc.dt.i=ac;
	_sys_argv=malloc(ac*sizeof(sll_runtime_object_t*));
	for (sll_integer_t i=0;i<ac;i++){
		*(_sys_argv+i)=SLL_ACQUIRE_STATIC(str_zero);
	}
}



__SLL_FUNC void sll_set_argument(sll_integer_t i,const char* a){
	if (i<0||i>=_sys_argc.dt.i){
		return;
	}
	sll_string_length_t l=0;
	sll_string_checksum_t c=0;
	while (*(a+l)){
		c^=*(a+l);
		l++;
	}
	SLL_RELEASE(*(_sys_argv+i));
	sll_runtime_object_t* n=SLL_CREATE();
	n->t=SLL_RUNTIME_OBJECT_TYPE_STRING;
	sll_string_create(l,&(n->dt.s));
	n->dt.s.c=c;
	memcpy(n->dt.s.v,a,l);
	*(_sys_argv+i)=n;
}



__API_FUNC(sys_arg_get){
	if (!ac){
		SLL_RETURN_ZERO_STRING;
	}
	const sll_runtime_object_t* e=*a;
	if (e->t!=SLL_RUNTIME_OBJECT_TYPE_INT||e->dt.i<0||e->dt.i>=_sys_argc.dt.i){
		SLL_RETURN_ZERO_STRING
	}
	SLL_ACQUIRE(*(_sys_argv+e->dt.i));
	return *(_sys_argv+e->dt.i);
}



__API_FUNC(sys_arg_get_count){
	SLL_ACQUIRE(&_sys_argc);
	return &_sys_argc;
}



__API_FUNC(sys_get_platform){
	if (!_sys_p.dt.s.l){
		sll_string_length_t l=0;
		sll_string_checksum_t c=0;
		while (*(sll_platform_string+l)){
			c^=*(sll_platform_string+l);
			l++;
		}
		sll_string_create(l,&(_sys_p.dt.s));
		_sys_p.dt.s.c=c;
		memcpy(_sys_p.dt.s.v,sll_platform_string,l);
	}
	SLL_ACQUIRE(&_sys_p);
	return &_sys_p;
}

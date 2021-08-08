#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/constants.h>
#include <sll/core.h>
#include <sll/platform.h>
#include <sll/string.h>
#include <sll/types.h>
#include <stdlib.h>



sll_sys_arg_count_t _sys_argc=0;
sll_string_t** _sys_argv=NULL;



__SLL_FUNC void sll_set_argument_count(sll_sys_arg_count_t ac){
	if (_sys_argv){
		for (sll_sys_arg_count_t i=0;i<_sys_argc;i++){
			if (*(_sys_argv+i)!=&_zero_string){
				sll_string_release(*(_sys_argv+i));
			}
		}
		free(_sys_argv);
	}
	_sys_argc=ac;
	_sys_argv=malloc(ac*sizeof(sll_string_t*));
	for (sll_sys_arg_count_t i=0;i<ac;i++){
		*(_sys_argv+i)=&_zero_string;
	}
}



__SLL_FUNC void sll_set_argument(sll_sys_arg_count_t i,const char* a){
	if (i>=_sys_argc){
		return;
	}
	sll_string_length_t l=0;
	sll_string_checksum_t c=0;
	while (*(a+l)){
		c^=*(a+l);
		l++;
	}
	sll_string_t* s=sll_string_create(l);
	s->rc=1;
	s->c=c;
	for (sll_string_length_t j=0;j<l;j++){
		s->v[j]=*(a+j);
	}
	s->v[l]=0;
	*(_sys_argv+i)=s;
}



__API_FUNC(sys_arg_get){
	o->t=SLL_RUNTIME_OBJECT_TYPE_STRING;
	if (!ac||a->t!=SLL_RUNTIME_OBJECT_TYPE_INT||a->dt.i<0||a->dt.i>=_sys_argc){
		o->dt.s=&_zero_string;
	}
	else{
		o->dt.s=*(_sys_argv+a->dt.i);
	}
	o->dt.s->rc++;
}



__API_FUNC(sys_arg_get_count){
	o->t=SLL_RUNTIME_OBJECT_TYPE_INT;
	o->dt.i=_sys_argc;
}



__API_FUNC(sys_get_platform){
	sll_string_length_t l=0;
	sll_string_checksum_t c=0;
	while (*(sll_platform_string+l)){
		c^=*(sll_platform_string+l);
		l++;
	}
	sll_string_t* s=sll_string_create(l);
	s->rc=1;
	s->c=c;
	for (sll_string_length_t j=0;j<l;j++){
		s->v[j]=*(sll_platform_string+j);
	}
	s->v[l]=0;
	o->t=SLL_RUNTIME_OBJECT_TYPE_STRING;
	o->dt.s=s;
}
